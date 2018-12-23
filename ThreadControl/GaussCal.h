#pragma once
#include "iostream"
#include "cv.h"
#include "math.h"
#include "omp.h"
#include "opencv2/opencv.hpp"
#include "highgui.h"
#include <vector>
#include <stdio.h> 
#include <fstream> 

using namespace std;
using namespace cv;

//Information for Every Row
typedef struct
{
	int x; //the estimated center 
	int y; //the row location
	int brightness; //the greatest brightness in real
	double cx; //Gaussfit center(Final Result)
	double cy; //the row location
	int bright; //The Greatest brightness in GaussFit
	int Rows; //the total num of rows(only saved in point[0])
}MPoint;

//The information of GaussPoint
typedef struct
{
	int x; // The Cols Location
	int brightness; // The Pixel Bright
}GPoint;

//The param of GetGaussCenter
typedef struct
{
	Mat matImage; //input image
	MPoint *point; //output result
	double maxError = 0.05; //the error to top for Gausspoint selection
	double minError = 0.1; //the error to bottom for Gausspoint selection
	int xRange = 20; //horizontal range base on the estimated center for Gausspoint selection
	int threads = 2; //threads used in OpenMP for Calculation
}GaussCalParam;

//the param of GetGaussIdentify
typedef struct
{
	Mat matImage; //input image
	MPoint *point; //input result of Cal
	double doorin = 0.39; //the gate you can choose
}GaussIdentifyParam;

typedef struct
{
	Mat matImage; //input image
	MPoint *point; //input result of Cal
}GaussManuIdenParam;

class GaussCal {
public:
	//Sub-Pixel center-line detection algorithm base on GaussFit
	void GaussCenter(GaussCalParam &guasscalparam);

	//Automatical Identification for error base on the gate you can choose
	void GaussIdentify(GaussIdentifyParam gaussidentifyparam);

	//Manual indentification for error by mouse click
	void GaussManuIden(GaussManuIdenParam gaussmanuidenparam);

	

private:
	//calculate the n_th of x
	int fx(int x, int n);

	//generate x and z Cvmat 
	int getXZmatrix(CvMat* X, CvMat* Z, int n, GPoint *gpoint);

	int bound(short i, short a, short b);

	//On_Mouse_Callback
	static void on_mouse(int event, int x, int y, int flags, void* ustc);

	//The reverse of CvScarlar
	CvScalar getInverseColor(CvScalar c);

	Mat src;

	Mat dst;

	Mat ori;

	int n = 0;

	vector<Point> points;
};