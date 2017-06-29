//#include <iostream>
//#include <opencv2/core/core.hpp>
//
//struct eqnline
//{
//	float m;
//	float b;
//};
//
//eqnline det_line_eqn(float x1, float y1, float x2, float y2)
//{
//	eqnline line;
//	float m, b;
//	m = (y2 - y1) / (x2 - x1); // determine slope
//	b = y1 - m*x1; // determine y-intercept
//	line = { m, b };
//	return line;
//}
//
//float x_POI(eqnline line1, eqnline line2)
//{
//	float xPOI;
//	xPOI = (line1.b - line2.b) / (line2.m - line1.m);
//	return(xPOI);
//}
//
//float cvert_POI(eqnline line, float imcenter)
//{
//	float xINT;
//	xINT = (line.m*imcenter) + line.b;
//	return(xINT);
//}
//
////int main()
////{
////	float x1, y1, x2, y2;
////	float x3, y3, x4, y4;
////	float x_pt;
////	eqnline line1, line2;
////
////	/*x1 = 1; y1 = 1;
////	x2 = 3; y2 = 3;
////
////	x3 = 0; y3 = 4;
////	x4 = 2; y4 = 8;*/
////	
////	x1 = (float)-0.9; y1 = (float)1.3;
////	x2 = 3.6; y2 = 14.8;
////
////	x3 = 15; y3 = 4.25;
////	x4 = 0; y4 = 8;
////
////	// Determine Equations of Lines
////	line1 = det_line_eqn(x1, y1, x2, y2);
////	line2 = det_line_eqn(x3, y3, x4, y4);
////
////	cout << "Line 1: Slope= " << line1.m << " Intercept= " << line1.b << endl;
////	cout << "Line 2: Slope= " << line2.m << " Intercept= " << line2.b << endl;
////
////	x_pt = x_POI(line1, line2);
////
////	cout << "X Point of Intersection = " << x_pt << endl;
////}
////
