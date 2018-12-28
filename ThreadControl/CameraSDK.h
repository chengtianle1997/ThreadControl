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
	int devNum = 0; //device No.
	int in_w; 
	int in_h;
	float ExposureTime = 13000;
	int ExposureAuto = 0;
	float Gain = 15;
	int GainAuto = 0;
	float AcquisitionFrameRate = 60; //The value is in effect when Trigger Mode is off
	//int DeviceId = 0; 
	int DeviceUserId = 0;
	int ROIHeight = 2048;
	int ROIWidth = 2592;
	int ROIOffsetX = 0;
	int ROIOffsetY = 0;
	int FrameSpecInfoSelector = 0;
	int FrameSpecInfo = 0;

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

	//Set Exposureauto
	int SetExposureAuto(int newExposureAuto);

	//Set AcquisitionFrameRate
	int SetAcquisitionFrameRate(float newAcquisitionFrameRate);

	//Set Gain
	int SetGain(float newGain);

	//Set GainAuto[GainSelector]
	int SetGainAuto(int newGainAuto);

	//Set DeviceUserID
	int SetDeviceUserID(const char* newDeviceUserID);

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