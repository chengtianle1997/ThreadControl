
#define _CRT_SECURE_NO_WARNINGS

#include "main.h"
#include <thread>
#include <mutex>

int CameraInit(CameraInitParam &camerainitparam)
{
	int ret;

	ret = camera.GetDevList();
	if (ret)
	{
		return ret;
	}
	printf("Please choose a camera:");
	scanf("%d", &camerainitparam.devNum);
	ret = camera.OpenCamera(camerainitparam);
	if (ret)
	{
		return ret;
	}
	ret = camera.StartGrabbing();
	if (ret)
	{
		return ret;
	}

	//Set prop
	ret = camera.SetGain(15.0);
	if (ret)
	{
		return ret;
	}
	ret = camera.SetExposureTime(15000);
	if (ret)
	{
		return ret;
	}
	ret = camera.SetAcquisitionFrameRate(60.0);
	if (ret)
	{
		return ret;
	}
	return 0;
}

int CameraClean()
{
	int ret;
	ret = camera.StopGrabbing();
	if (ret)
	{
		return ret;
	}
	ret = camera.CloseDevice();
	if (ret)
	{
		return ret;
	}
}


void EncoderInit(EncoderParam encoderparam)
{
	encoder.Init(encoderparam);
}

void EncoderClean()
{
	encoder.Clean();
}


int main(int argc,char* argv[])
{
	CameraInitParam camerainitparam;
	EncoderParam encoderparam;
	CameraInit(camerainitparam);
	
	encoderparam.in_w = camerainitparam.in_w;
	encoderparam.in_h = camerainitparam.in_h;
	//printf("%dx%d", encoderparam.in_w, encoderparam.in_h);
	EncoderInit(encoderparam);


	return 0;
}