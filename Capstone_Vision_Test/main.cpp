#include "stop_marker_detection.h"
#include "generalFunc.h"

void test_StopMarkerDistanceDetect();

int main()
{
	test_StopMarkerDistanceDetect(); // test stop marker distance detection
}

void test_StopMarkerDistanceDetect()
{
	// Load And Display Image
		// File Path and File Name of Image
		std::string impath = "C:\\Pictures\\";
		std::string fname = "binaryImage3m5MP-pp_binary.bmp";
		// Read image in
		cv::Mat imraw = cv::imread(impath + fname, cv::IMREAD_COLOR);		// for color
		// cv::Mat imraw = cv::imread(impath + fname, cv::IMREAD_GRAYSCALE);	// for grayscale
		// Display Image
		cv::Mat imrawview;
		cv::pyrDown(imraw, imrawview);
		cv::imshow("Raw Input Image", imrawview);
		cv::waitKey(0);

	// Find Object Distance and Print to Console
		double dist;
		// Object and Camera Parameters
		const double objWidth = 5.08; // meters
		const double fl = 192.22; // meters for 2592x1458
		// const double fl = 96.11; // meters for 1280x720
		dist = get_objdist(imraw, objWidth, fl);
		std::cout << "Distance is " << dist << std::endl; // Print Result

	// Close All Windows
		cv::destroyAllWindows();
}