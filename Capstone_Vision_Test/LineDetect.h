#pragma once
// Library
#include <iostream>
#include <vector>
#include <opencv2/core/core.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/features2d/features2d.hpp>
// Self
#include "generalFunc.h"

struct line_eqn
{
	double m; // slope
	double b; // intercept
	double x1;// endpoint 1 x coordinate
	double y1;// endpoint 1 y coordinate
	double x2;// endpoint 2 x coordinate
	double y2;// endpoint 2 y coordinate
};

// Detect Line End Points From Image
std::vector<line_eqn> line_detect_hough(cv::Mat im, double minLength, double maxGap);

// Derive Equation of a Line in Slope-Intercept Form from 2 Points of Line
line_eqn det_line_eqn(double x1, double y1, double x2, double y2);

// Draw Lines on Image
void drawAllLines(cv::Mat& imColor, std::vector<line_eqn> lines, int drawLimit);
std::vector<line_eqn> drawAllLines_return(cv::Mat& imColor, std::vector<line_eqn> lines, int drawLimit);
void drawAllLines_wSlopes(cv::Mat& imColor, std::vector<line_eqn> lines, int drawLimit);

// Filter Lines
std::vector<line_eqn> filterLines(std::vector<line_eqn> ext_lines, double slopecutoffHi, double slopecutoffLo);
float limiter(float val, float upplim, float lowlim);

//Get Steering Angle
float getStrAng(cv::Mat im, cv::Mat im2);

// Draw text at midpoint of line
void drawAtMidpoint(cv::Mat& im, std::string text, line_eqn line);