#pragma once
#include "iostream"
#include "stdio.h"
#include <process.h>
#include "afxwin.h"
#include  "cv.h"
#include "opencv2/opencv.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "thread"
#include "MvCameraControl.h"

using namespace cv;
using namespace std;

typedef struct {
	int devNum; //device num
	int in_w; 
	int in_h;
	float AcquisitionFrameRate; //The value is in effect when Trigger Mode is off
}CameraInitParam;


class Camera {
public:
	//Get Device List
	int GetDevList();

	//Open Camera 
	int OpenCamera(CameraInitParam &camerainitparam);

	//Start Grabbing
	int StartGrabbing();

	//Set ExposureTime 
	int SetExposureTime(float newExposureTime);

	//Set AcquisitionFrameRate
	int SetAcquisitionFrameRate(float newAcquisitionFrameRate);

	//Set Gain
	int SetGain(float newGain);

	//Get Image
	int GetImage(MV_FRAME_OUT &stOutFrame);

	//Stop Grabing Image
	int StopGrabbing();

	//Close Device
	int CloseDevice();

private:
	int nRet = MV_OK;  //Return

	MV_CC_DEVICE_INFO_LIST stDevList;  //Device_List

	//MV_FRAME_OUT stOutFrame = { 0 }; //Frame_Out

	void* handle = NULL;  //Camera_Handle

	unsigned int g_nPayloadSize = 0;

	//Print Device Info
	bool  PrintDeviceInfo(MV_CC_DEVICE_INFO* pstMVDevInfo);

};