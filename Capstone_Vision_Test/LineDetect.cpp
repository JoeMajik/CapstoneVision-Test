#include "LineDetect.h"

/*
TODO:
 1. Derive a steering angle from the detected line (setup logic)
 2. Add y intercept (b) filtering
 3. Make more reliable
 */

// Vision Steering Determination
float getStrAng(cv::Mat im, cv::Mat im2)
{
	float StrAng = 0;

	// Stage 1: Detect Lines on Image as Endpoints
	std::vector<line_eqn> extracted_lines = line_detect_hough(im, 100, 1000);

	// Stage 2: Filter the Lines Based on Slope and Other Factors
	//std::vector<line_eqn> filtered_lines = filterLines(extracted_lines, 10000, -10000);

	// Stage 3: Compute Turning Angle from Point of Intersection
	int imCenter = im.rows / 2; // center of image for POI
	//cvert_POI(eqn, imCenter);

	// TESTING ONLY
	// Draw Results
	cv::Mat im3 = im2.clone();
	std::vector<line_eqn> firstSet = drawAllLines_return(im, extracted_lines, 10); // draw first X lines from set and return only them
	cv::pyrDown(im, im);
	cv::imshow("All The Lines", im);
	firstSet = filterLines(firstSet, -0.31, -0.33); // filter the X amount of lines
	drawAllLines(im2, firstSet, firstSet.size());	// draw lines that pass the filter
	drawAllLines_wSlopes(im3, firstSet, firstSet.size()); // draw them with slopes
	
	cv::pyrDown(im2, im2);
	cv::imshow("Filtered Set Lines", im2);
	cv::pyrDown(im3, im3);
	cv::imshow("Filtered Set Lines w slopes", im3);
	cv::waitKey(0);
	cv::destroyAllWindows();
	return StrAng;
}

// Detects the Lines on the Input Image based on Hough Transform
std::vector<line_eqn> line_detect_hough(cv::Mat im, double minLength, double maxGap) // max = 100, min = 30
{ 
	const double pi = 3.141592653589793238463;
	std::vector<cv::Vec4i> lines;
	std::vector<line_eqn> eqns;
	// Convert to Grayscale
	cv::Mat imB;
	cv::cvtColor(im, imB, cv::COLOR_BGR2GRAY); // convert to grayscale

	// Blur Image
	cv::Mat imC = imB.clone();
	//cv::GaussianBlur(imB, imC, cv::Size(5,5),3,3);
	cv::blur(imB, imC, cv::Size(5, 5));

	// Perform Edge Detection
	cv::Canny(imC,imC,100,200,3);

	// Use Hough Transform to Find Lines (as endpoints)
	cv::HoughLinesP(imC, lines, 1, pi / 180, 15, minLength, maxGap);

	// Convert End Points into Equations of Line
	eqns.resize(lines.size());
	for (int i = 0; i < lines.size(); i++)
		eqns[i] = det_line_eqn(lines[i][0], lines[i][1], lines[i][2], lines[i][3]); // convert line endpoints into equations of lines
	return eqns;
}

// LINE FILTERING FUNCTIONS
std::vector<line_eqn> filterLines(std::vector<line_eqn> ext_lines, double slopecutoffHi, double slopecutoffLo)
{
	std::vector<line_eqn> lines;
	for (int i = 0; i < ext_lines.size(); i++)
	{
		if (ext_lines[i].m < 0 && ext_lines[i].m < slopecutoffHi && ext_lines[i].m > slopecutoffLo) // filter by slope (remove positive slopes to only get left side)
			lines.push_back(ext_lines[i]);
	}
	return lines;
}

// LINE DRAWING FUNCTIONS
void drawAllLines(cv::Mat& imColor, std::vector<line_eqn> lines, int drawLimit)
{
	for (int i = 0; i < drawLimit;i++)
		cv::line(imColor, cv::Point(lines[i].x1, lines[i].y1), cv::Point(lines[i].x2, lines[i].y2), cv::Scalar(0, 0, 255), 7, 8);
}

std::vector<line_eqn> drawAllLines_return(cv::Mat& imColor, std::vector<line_eqn> lines, int drawLimit)
{
	std::vector<line_eqn> drawnLines;
	for (int i = 0; i < drawLimit; i++)
	{
		cv::line(imColor, cv::Point(lines[i].x1, lines[i].y1), cv::Point(lines[i].x2, lines[i].y2), cv::Scalar(0, 0, 255), 7, 8);
		drawnLines.push_back(lines[i]);
	}
	return drawnLines;
}

void drawAllLines_wSlopes(cv::Mat& imColor, std::vector<line_eqn> lines, int drawLimit)
{
	for (int i = 0; i < drawLimit; i++)
	{
		cv::line(imColor, cv::Point(lines[i].x1, lines[i].y1), cv::Point(lines[i].x2, lines[i].y2), cv::Scalar(0, 0, 255), 7, 8);
		drawAtMidpoint(imColor, std::to_string(lines[i].m), lines[i]);
	}
}

// LINE EQUATION FUNCTIONS
// Compute Equation of a Line from Endpoints
line_eqn det_line_eqn(double x1, double y1, double x2, double y2)
{
	line_eqn line;
	double m, b;
	m = (y2 - y1) / (x2 - x1); // determine slope
	b = y1 - m*x1; // determine y-intercept
	line = {m, b, x1, y1, x2, y2};
	return line;
}

// Compute Horizontal (x) Interception Point between Two Lines
double x_POI(line_eqn line1, line_eqn line2)
{
	double xPOI;
	xPOI = (line1.b - line2.b) / (line2.m - line1.m);
	return(xPOI);
}

// Compute Vertical (y) Intersection with Center Line of Image 
double cvert_POI(line_eqn line, int imcenter)
{
	double xINT;
	xINT = (line.m*(double)imcenter) + line.b;
	return(xINT);
}

// Logic for Determining Steering Angle Based on Central Intersection
float getSteeringAng(int vertInt, int cthresh, int center, int othresh)
{
	float strAng = 0;

	if (vertInt < center + cthresh && vertInt > center - cthresh)
	{
		strAng = 0; // within central bound, continue straight
	}
	else if (vertInt > center + othresh && vertInt < center - othresh)
	{
		strAng = 0; // outside of possible range, set steering to 0
	}
	else if (vertInt > center + cthresh) 
	{
		// turn right
		strAng = exp2f(abs(vertInt - center)); // change to linear
		strAng = limiter(strAng, 1, -1);
	}
	else if (vertInt < center - cthresh)
	{
		// turn left
		strAng = exp2f(abs(vertInt - center)); // change to linear
		strAng = limiter(strAng, 1, -1);
	}
	return strAng;
}

// USEFUL FUNCTIONS
// this function keeps an input value set within set upper and lower values
float limiter(float val, float upplim, float lowlim)
{
	if (upplim < lowlim) return 0; // invalid input
	if (val > upplim)	return upplim;	// if upper limit exceeded set value to upper limit
	if (val < lowlim)	return lowlim;	// if lower limit exceeded set value to lower limit
}

// draws text at midpoint of line
void drawAtMidpoint(cv::Mat& im, std::string text, line_eqn line)
{
	double mpx = (line.x1 + line.x2) / 2;
	double mpy = (line.y1 + line.y2) / 2;
	cv::Point mp = { (int) mpx, (int) mpy};
	cv::putText(im, text, mp, cv::FONT_HERSHEY_SCRIPT_SIMPLEX, 3, cv::Scalar(255, 0, 0), 5, 8);
}