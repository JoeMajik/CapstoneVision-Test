#include "LineDetect.h"

struct line_eqn
{
	double m; // slope
	double b; // intercept
};

// detects the lines on the binary map based on hough transform
//std::vector<line_eqn> line_detect_hough()
//{
//
//}
//
//// detects the lines on the binary map using horizonatal detection and linear interpolation
//std::vector<line_eqn> line_detect_linint()
//{
//
//}

line_eqn det_line_eqn(double x1, double y1, double x2, double y2)
{
	line_eqn line;
	double m, b;
	m = (y2 - y1) / (x2 - x1); // determine slope
	b = y1 - m*x1; // determine y-intercept
	line = { m, b };
	return line;
}

//compute horizontal (x) interception point between two lines
double x_POI(line_eqn line1, line_eqn line2)
{
	double xPOI;
	xPOI = (line1.b - line2.b) / (line2.m - line1.m);
	return(xPOI);
}

// compute vertical intersection with center line of image 
double cvert_POI(line_eqn line, int imcenter)
{
	double xINT;
	xINT = (line.m*(double)imcenter) + line.b;
	return(xINT);
}