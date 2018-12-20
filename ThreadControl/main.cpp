
#define _CRT_SECURE_NO_WARNINGS

#include "main.h"
#include <thread>
#include <mutex>

mutex m;

GaussCalParam Calparam;

GaussIdentifyParam IdentifyParam;

CameraInitParam camerainitparam;

EncoderParam encoderparam;

EncodeParam encodeparam;

//State of occupation
BOOL Buffer0Mutex = 0;
BOOL Buffer1Mutex = 0;

//Indicate got image
BOOL GetImage0 = 0;
BOOL GetImage1 = 0;

//Indicate Calculation end
BOOL CalEnd0 = 0;
BOOL CalEnd1 = 0;

//Indicate the require to encode
BOOL CodeState0 = 0;
BOOL CodeState1 = 0;


//The double buffer
unsigned char* Buffer0;
unsigned char* Buffer1;
//The information of buffer
//MV_FRAME_OUT_INFO_EX Buffer0Info;
//MV_FRAME_OUT_INFO_EX Buffer1Info;

int SizeofPixels = 0;
int ImageWidth = 0;
int ImageHeight = 0;

//The Exit Instruction(Set 1 to Stop)
int AcqExit = 0;
int CalExit = 0;
int EncodeExit = 0;

//Functional choice
int ResVisAuto = 0;
int ResVisManu = 0;

int PerforFramecnt = 0;


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
	//Set prop
	ret = camera.SetGain(15.0);
	if (ret)
	{
		return ret;
	}
	ret = camera.SetExposureTime(13000);
	if (ret)
	{
		return ret;
	}
	ret = camera.SetAcquisitionFrameRate(60.0);
	if (ret)
	{
		return ret;
	}
	//Start Grabing
	ret = camera.StartGrabbing();
	if (ret)
	{
		return ret;
	}

	ImageWidth = camerainitparam.in_w;
	ImageHeight = camerainitparam.in_h;
	SizeofPixels = camerainitparam.in_w*camerainitparam.in_h;

	//Malloc for double buffer
	Buffer0 = (unsigned char*)malloc(camerainitparam.in_w*camerainitparam.in_h);
	Buffer1 = (unsigned char*)malloc(camerainitparam.in_w*camerainitparam.in_h);

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
	free(Buffer0);
	free(Buffer1);
}


void EncoderInit(EncoderParam encoderparam)
{
	encoder.Init(encoderparam);
}

void EncoderClean()
{
	encoder.Clean();
}

void CalImageThread()
{
	//Find buffer to cal
	while (!CalExit) {
		lock_guard<mutex> lockGuard(m);
		if (GetImage0 && !CalEnd0 && !Buffer0Mutex)
		{
			Mat matImage{
				cvSize(ImageWidth,ImageHeight),
				CV_8UC1,
				Buffer0
			};
			GetImage0 = 0;
			Calparam.matImage = matImage;
			Calparam.point = new MPoint[ImageHeight];
			Gausscal.GaussCenter(Calparam);
			//Process the Result
			if (ResVisAuto) {

				IdentifyParam.matImage = matImage;
				IdentifyParam.point = Calparam.point;
				Gausscal.GaussIdentify(IdentifyParam);

			}
			if (ResVisManu) {

				GaussManuIdenParam ManuIdenParam;
				ManuIdenParam.matImage = matImage;
				ManuIdenParam.point = Calparam.point;
				Gausscal.GaussManuIden(ManuIdenParam);

			}
			CalEnd0 = 1;
			//Performance Check
			PerforFramecnt++;
			delete(Calparam.point);
		}
		else if (GetImage1 && !CalEnd1 && !Buffer1Mutex) 
		{
			Mat matImage{
				cvSize(ImageWidth,ImageHeight),
				CV_8UC1,
				Buffer1
			};
			GetImage1 = 0;
			Calparam.matImage = matImage;
			Calparam.point = new MPoint[ImageHeight];
			Gausscal.GaussCenter(Calparam);

			//Process the Result
			if (ResVisAuto) {

				IdentifyParam.matImage = matImage;
				IdentifyParam.point = Calparam.point;
				Gausscal.GaussIdentify(IdentifyParam);

			}
			if (ResVisManu) {

				GaussManuIdenParam ManuIdenParam;
				ManuIdenParam.matImage = matImage;
				ManuIdenParam.point = Calparam.point;
				Gausscal.GaussManuIden(ManuIdenParam);

			}
			CalEnd1 = 1;
			//Performance Check
			PerforFramecnt++;
			delete(Calparam.point);
		}

	}
	
}

void AcqImageThread()
{

	MV_FRAME_OUT stOutFrame;
	memset(&stOutFrame, 0, sizeof(MV_FRAME_OUT));

	CalEnd0 = 1;
	CalEnd1 = 1;

	int Framenum;

	while (!AcqExit) {

		//stOutFrame = (MV_FRAME_OUT*)malloc(sizeof(MV_FRAME_OUT));
		camera.GetImage(stOutFrame);
		//Decide the Buffer 
		lock_guard<mutex> lockGuard(m);
		if (CalEnd0)
		{
			Buffer0Mutex = 1;
			memcpy(Buffer0, (stOutFrame).pBufAddr, SizeofPixels);
			Buffer0Mutex = 0;
			//Buffer0Info = stOutFrame.stFrameInfo;
			Framenum = stOutFrame.stFrameInfo.nFrameNum;
			//Decide if Encode
			if (Framenum % (encodeparam.FrameCut + 1) == 0)
				CodeState0 = 1;
			CalEnd0 = 0;
			GetImage0 = 1;
		}
		else if (CalEnd1)
		{
			Buffer1Mutex = 1;
			memcpy(Buffer1, (stOutFrame).pBufAddr, SizeofPixels);
			Buffer1Mutex = 0;
			//Buffer1Info = stOutFrame.stFrameInfo;
			Framenum = stOutFrame.stFrameInfo.nFrameNum;
			//Decide if Encode
			if (Framenum % (encodeparam.FrameCut + 1) == 0)
				CodeState1 = 1;
			CalEnd1 = 0;
			GetImage1 = 1;
		}
	}
}

void EncodeThread()
{
	int pts = 0; //inner frame counter 
	while (!EncodeExit) {
		if (CodeState0 && !Buffer0Mutex)
		{
			encodeparam.pBufAddr = Buffer0;
			encodeparam.pts = pts;
			encoder.Encode(encodeparam);
			pts++;
			CodeState0 = 0;
		}
		else if (CodeState1 && !Buffer1Mutex)
		{
			encodeparam.pBufAddr = Buffer1;
			encodeparam.pts = pts;
			encoder.Encode(encodeparam);
			pts++;
			CodeState1 = 0;
		}
	}
	 
}

void TimerPerformance()
{
	watch.start();
	while (1) {
		watch.stop();
		if (watch.elapsed() > 1000000)
		{
			printf("The Framerate is %d fps\n", PerforFramecnt);
			watch.restart();
			PerforFramecnt = 0;
		}
		else {
			watch.start();
			Sleep(10);
		}
	}
	 
}

int main(int argc,char* argv[])
{
	int ret;
	camerainitparam.AcquisitionFrameRate = 60.0;
	ret = CameraInit(camerainitparam);
	if (ret) {
		printf("Camera Init failed\n");
	}
	
	encoderparam.in_w = camerainitparam.in_w;
	encoderparam.in_h = camerainitparam.in_h;
	//printf("%dx%d", encoderparam.in_w, encoderparam.in_h);
	EncoderInit(encoderparam);
	printf("Please input the thread num:");
	scanf("%d", &Calparam.threads);
	//set the calculation param
	Calparam.maxError = 0.05;
	Calparam.minError = 0.10;
	//Calparam.threads = 2;
	Calparam.xRange = 20;

	//set the identify param
	IdentifyParam.doorin = 0.39;

	//Encodeparam
	encodeparam.CameraNum = 0;
	encodeparam.FrameCut = 5;
	encodeparam.nHeight = ImageHeight;
	encodeparam.nWidth = ImageWidth;


	//AcqImageThread();
	//watch.start();

	thread acqthread(AcqImageThread);

	thread calthread(CalImageThread);

	thread Timer(TimerPerformance);

	thread Encoder(EncodeThread);

	acqthread.join();

	calthread.join();

	Timer.join();

	Encoder.join();

	return 0;
}