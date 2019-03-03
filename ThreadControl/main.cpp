
#define _CRT_SECURE_NO_WARNINGS 

//#define ENCODE_ENABLE

#include "main.h"

ofstream outFile;


int CameraInit(CameraInitParam &camerainitparam)
{
	int ret;

	ret = camera.GetDevList();
	if (ret)
	{
		return ret;
	}
	//printf("Please choose a camera:");
	//scanf("%d", &camerainitparam.devNum);
	ret = camera.OpenCamera(camerainitparam);
	if (ret)
	{
		return ret;
	}

	if (!camerainitparam.ROIWidth)
	{
		camerainitparam.ROIWidth = camerainitparam.in_w;
	}
	if (!camerainitparam.ROIHeight)
	{
		camerainitparam.ROIHeight = camerainitparam.in_h;
	}

	//Get Device info
	camera.GetDevInfo(camerainitparam.devNum, camerainitparam.DevInfo);
	if (ret)
	{
		printf("Get DeviceInfo Failed");
	}

	//Set prop
	//Set Exposureauto
	ret = camera.SetExposureAuto(camerainitparam.ExposureAuto);
	if (ret)
	{
		return ret;
	}
	if (!camerainitparam.ExposureAuto)
	{
		//Set ExposureTime 
		ret = camera.SetExposureTime(camerainitparam.ExposureTime);
		if (ret)
		{
			return ret;
		}
	}
	
	
	//Set AcquisitionFrameRate
	ret = camera.SetAcquisitionFrameRate(camerainitparam.AcquisitionFrameRate);
	if (ret)
	{
		return ret;
	}
	

	//Set GainAuto[GainSelector]
	ret = camera.SetGainAuto(camerainitparam.GainAuto);
	if (ret)
	{
		return ret;
	}
	if (!camerainitparam.GainAuto)
	{
		//Set Gain
		ret = camera.SetGain(camerainitparam.Gain);
		if (ret)
		{
			return ret;
		}
	}
	
	
	//Set Width
	ret = camera.SetWidth(camerainitparam.ROIWidth);
	if (ret)
	{
		return ret;
	}
	//Set Height
	ret = camera.SetHeight(camerainitparam.ROIHeight);
	if (ret)
	{
		return ret;
	}
	//Set OffsetX
	ret = camera.SetOffsetX(camerainitparam.ROIOffsetX);
	if (ret)
	{
		return ret;
	}
	//Set OffsetY
	ret = camera.SetOffsetY(camerainitparam.ROIOffsetY);
	if (ret)
	{
		return ret;
	}
	//Set FrameSpecInfoSelector
	ret = camera.SetFrameSpecInfoSelector(camerainitparam.FrameSpecInfoSelector);
	if (ret)
	{
		return ret;
	}
	//Set FrameSpecInfo
	ret = camera.SetFrameSpecInfo(camerainitparam.FrameSpecInfo);
	if (ret)
	{
		return ret;
	}
	//Set TriggerSelector
	ret = camera.SetTriggerSelector(camerainitparam.TriggerSelector);
	if (ret)
	{
		return ret;
	}
	//Set TriggerMode
	ret = camera.SetTriggerMode(camerainitparam.TriggerMode);
	if (ret)
	{
		return ret;
	}
	//Set TriggerSource
	ret = camera.SetTriggerSource(camerainitparam.TriggerSource);
	if (ret)
	{
		return ret;
	}
	//Set TriggerActivation
	ret = camera.SetTriggerActivation(camerainitparam.TriggerActivation);
	if (ret)
	{
		return ret;
	}
	//Set TriggerDelay
	ret = camera.SetTriggerDelay(camerainitparam.TriggerDelay);
	if (ret)
	{
		return ret;
	}
	//Set TriggerCacheEnable
	ret = camera.SetTriggerCacheEnable(camerainitparam.TriggerCacheEnable);
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
	int ret =0;
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
	return 1;
}


void EncoderJPEGInit(EncoderParam encoderparam)
{
	encoder.InitJPEG(encoderparam);
}

void EncoderJPEGClean()
{
	encoder.JPEGClean();
}

void EncoderMJPEGInit(EncoderParam encoderparam)
{
	encoder.InitMJPEG(encoderparam);
}

void EncoderMJPEGClean()
{
	encoder.MJPEGClean();
}

void EncoderFlushMJPEG()
{
	encoder.MJPEGFlush();
}

void ClientInit()
{
	int ret;

	printf("The Client is connecting.....\n");

	//int Serverport;

	Serverport = clientparam.StartUpPort + camerainitparam.devNum;

	ret = client.ClientConnect(Serverport, clientparam.ServerAddr);

	if (ret)
	{
		printf("Connecting failed!");
	}
}

void ClientClean() {
	printf("Closing Socket......\n");
	client.ClientClose();
}

int CreateFolder(const char* filepath, char* finalpath, int Devnum)
{
	int nRet;
	nRet = filemanager.CreateFolder(filepath, finalpath, Devnum);
	
	return nRet;
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

			//Package the Data
			SocketPackage PackageData0;
			for (int i = 0; i < INFO_MAX_BUFFER_SIZE; i++)
			{
				PackageData0.SerialNumber[i] = camerainitparam.DevInfo.SpecialInfo.stUsb3VInfo.chSerialNumber[i];
			}
			
			PackageData0.Framecnt = Buffer0Info.nFrameNum;
			for (int i = 0; i < ImageHeight; i++)
			{
				PackageData0.s[i] = Calparam.point[i].s;
				PackageData0.ay[i] = Calparam.point[i].ay;
			}

			outFile << PackageData0.SerialNumber << "," << PackageData0.Framecnt;
			outFile << endl;
				
			//SendQueue.push(PackageData0);
			//int q = SendQueue.size();
			if (EnableSendData) 
			{
				char* buf = (char*)malloc(sizeof(SocketPackage));

				memcpy(buf, &PackageData0, sizeof(SocketPackage));

				client.ClientSend(buf, sizeof(SocketPackage));

				free(buf);

				
			}
			
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

			//Package the Data
			SocketPackage PackageData1;
			for (int i = 0; i < INFO_MAX_BUFFER_SIZE; i++)
			{
				PackageData1.SerialNumber[i] = camerainitparam.DevInfo.SpecialInfo.stUsb3VInfo.chSerialNumber[i];
			}

			PackageData1.Framecnt = Buffer0Info.nFrameNum;
			for (int i = 0; i < ImageHeight; i++)
			{
				PackageData1.s[i] = Calparam.point[i].s;
				PackageData1.ay[i] = Calparam.point[i].ay;
			}

			outFile << PackageData1.SerialNumber << "," << PackageData1.Framecnt;
			outFile << endl;

			//SendQueue.push(PackageData1);
			if (EnableSendData)
			{
				char* buf = (char*)malloc(sizeof(SocketPackage));

				memcpy(buf, &PackageData1, sizeof(SocketPackage));

				client.ClientSend(buf, sizeof(SocketPackage));

				
			}
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
			Buffer0Info = stOutFrame.stFrameInfo;
			Framenum = stOutFrame.stFrameInfo.nFrameNum;
			
			
			if (CalEnable) {
				CalEnd0 = 0;
				GetImage0 = 1;
			}

			if (EncodeEnable) {
				//Decide if Encode
				if (Framenum % (encodeparam.FrameCut + 1) == 0)
				{
					CodeState0 = 1;
					GetImage0 = 0;
					CalEnd0 = 1;
				}
					
			
			}
			
			
		}
		else if (CalEnd1)
		{
			Buffer1Mutex = 1;
			memcpy(Buffer1, (stOutFrame).pBufAddr, SizeofPixels);
			Buffer1Mutex = 0;
			Buffer1Info = stOutFrame.stFrameInfo;
			Framenum = stOutFrame.stFrameInfo.nFrameNum;
			if (CalEnable) {
				CalEnd1 = 0;
				GetImage1 = 1;
			}

			if (EncodeEnable) {
				//Decide if Encode
				if (Framenum % (encodeparam.FrameCut + 1) == 0)
				{
					CodeState1 = 1;
					GetImage1 = 0;
					CalEnd1 = 1;
				}


			}
		}
	}
}

void EncodeJPEGThread()
{
	int pts = 0; //inner frame counter 
	while (!EncodeExit) {
		if (CodeState0 && !Buffer0Mutex)
		{
			encodeparam.pBufAddr = Buffer0;
			encodeparam.pts = pts;
			encoder.EncodeJPEG(encodeparam);
			pts++;
			CodeState0 = 0;
			PerforFrameenc++;
		}
		else if (CodeState1 && !Buffer1Mutex)
		{
			encodeparam.pBufAddr = Buffer1;
			encodeparam.pts = pts;
			encoder.EncodeJPEG(encodeparam);
			pts++;
			CodeState1 = 0;
			PerforFrameenc++;
		}
	}
	 
}
void EncodeMJPEGThread()
{
	int pts = 0;
	while (!EncodeExit)
	{
		if (CodeState0 && !Buffer0Mutex)
		{
			encodeparam.pBufAddr = Buffer0;
			encodeparam.pts = pts;
			encoder.EncodeMJPEG(encodeparam);
			pts++;
			CodeState0 = 0;
			PerforFrameenc++;
		}
		else if (CodeState1 && !Buffer1Mutex)
		{
			encodeparam.pBufAddr = Buffer1;
			encodeparam.pts = pts;
			encoder.EncodeMJPEG(encodeparam);
			pts++;
			CodeState1 = 0;
			PerforFrameenc++;
		}
	}
}
//TIme to Test Performance
void TimerPerformance()
{			
	watch.start();
	while (!TimerExit) {
		watch.stop();
		if (watch.elapsed() > 1000000)
		{
			if (CalEnable&&!EncodeEnable) {
				printf("The Calculate Framerate is %d fps\n", PerforFramecnt);
			}
			else if (!CalEnable&&EncodeEnable) {
				printf("\n\n\n\nThe Encode Framerate is %d fps\n\n\n\n", PerforFrameenc);
			}
			else if (CalEnable&&EncodeEnable) {
				printf("\n\n\n\n\nThe Calculate Framerate is %d fps, \t The Encode Framerate is %d fps\n\n\n\n\n\n", PerforFramecnt,PerforFrameenc);
			}
			
			watch.restart();
			PerforFramecnt = 0;
			PerforFrameenc = 0;
		}
		else {
			watch.start();
			Sleep(10);
		}
	}
	 
}
//The Extra thread for Send
void SendResToPort() {

	while (!SendExit || !SendQueue.empty())
	{
		int i = SendQueue.size();
		printf("%d\n", i);
		if (!SendQueue.empty())
		{
			SocketPackage SockPack;

			SockPack = SendQueue.front();

			char* buf = (char*)malloc(sizeof(SocketPackage));

			memcpy(buf, &SockPack, sizeof(SocketPackage));

			client.ClientSend(buf, sizeof(SocketPackage));

			SendQueue.pop();

			free(buf);
		}
	}
}

//����ָֹͣ�����
BOOL __stdcall ConsoleHandler(DWORD cevent) {
	//char mesg[128];

	switch (cevent)
	{

	case CTRL_C_EVENT:
	{
		/*MessageBox(NULL, "ctrl+c received!", "cevent", MB_OK);*/

		AcqExit = 1;
		CalExit = 1;
		EncodeExit = 1;
		TimerExit = 1;
	}
		break;
	case CTRL_BREAK_EVENT:
		/*MessageBox(NULL,
			"ctrl+break received!", "cevent", MB_OK);*/
		break;
	case CTRL_CLOSE_EVENT:
		/*MessageBox(NULL,
			"program being closed!", "cevent", MB_OK);*/
	{
		AcqExit = 1;
		CalExit = 1;
		EncodeExit = 1;
		TimerExit = 1;

		/*int ret = 0;
		ret = CameraClean();
		if (ret) {
			printf("Camera Clean failed\n");
		}
		CameraClean();
		if (Format == 0)
		{
			EncoderMJPEGClean();
		}
		else if (Format == 1)
		{
			EncoderJPEGClean();
		}

		ClientClean();

		free(FinalPath);

		exit(0);*/
	}
		break;

	case CTRL_LOGOFF_EVENT:
		/*MessageBox(NULL,
			"user is logging off!", "cevent", MB_OK);*/
		break;
	case CTRL_SHUTDOWN_EVENT:
		/*MessageBox(NULL,
			"user is logging off!", "cevent", MB_OK);*/
		break;

	}
	return true;
}





int main(int argc,char* argv[])
{
	cmdline::parser args;

	// add specified type of variable.
	// 1st argument is long name
	// 2nd argument is short name (no short name if '\0' specified)
	// 3rd argument is description
	// 4th argument is mandatory (optional. default is false)
	// 5th argument is default value  (optional. it used when mandatory is false)
	// 6th argument is extra constraint.

	
	//COM port params
	//FUnctionChoice
	args.add<UINT>("function", 'f', "FunctionChoice", false, 1, cmdline::range(1, 9));
	//CameraParam
	args.add<FLOAT>("exptime", 'e', "CameraExposureTime", false, 13000, cmdline::range<FLOAT>(0, 1000000));
	args.add<UINT>("expauto", '\0', "CameraExposureAuto", false, 0, cmdline::range(0, 2));
	args.add<FLOAT>("gain", 'g', "CameraGain", false, 15, cmdline::range<FLOAT>(0, 15));
	args.add<UINT>("gainauto", '\0', "CameraGainAuto", false, 0, cmdline::range(0, 2));
	args.add<FLOAT>("frate", 'r', "CameraAcquisitionFrameRate", false, 60, cmdline::range<FLOAT>(0, 200));
	args.add<UINT>("devid", 'v', "CameraDeviceId", false, 0, cmdline::range(0, 10));
	args.add<string>("usrid", '\0', "DeviceUserId", false, "");
	args.add<UINT>("roih", '\0', "ROIHeight", false, 2048, cmdline::range(0, 2048));
	args.add<UINT>("roiw", '\0', "ROIWidth", false, 2592, cmdline::range(0, 2592));
	args.add<UINT>("roix", '\0', "ROIOffsetX", false, 0, cmdline::range(0, 2592));
	args.add<UINT>("roiy", '\0', "ROIOffsetY", false, 0, cmdline::range(0, 2048));
	args.add<UINT>("stampsel", '\0', "FrameSpecInfoSelector", false, 0, cmdline::range(0, 8));
	args.add<UINT>("stampon", '\0', "CameraFrameSpecInfo", false, 0, cmdline::range(0, 1));
	args.add<UINT>("tsele", '\0', "TriggerSelector", false, 6, cmdline::range(0, 12));
	args.add<UINT>("tmode", '\0', "TriggerMode", false, 0, cmdline::range(0, 1));
	args.add<UINT>("tsrc", '\0', "TriggerSource", false, 0, cmdline::range(0, 8));
	args.add<UINT>("tacti", '\0', "TriggerActivation", false, 0, cmdline::range(0, 3));
	args.add<FLOAT>("tdelay", '\0', "TriggerDelay", false, 0, cmdline::range<FLOAT>(0, 1000000));
	args.add<UINT>("tcaen", '\0', "TriggerCacheEnable", false, 0, cmdline::range(0, 1));
	//GaussCalParam
	args.add<FLOAT>("emax", 'a', "MaxError", false, 0.13, cmdline::range<FLOAT>(0, 1));
	args.add<FLOAT>("emin", 'i', "MinError", false, 0.13, cmdline::range<FLOAT>(0, 1));
	args.add<UINT>("xr", 'x', "XRange", false, 15, cmdline::range(0, 100));
	args.add<UINT>("gthread", 't', "GaussThread", false, 2, cmdline::range(1, 20));
	args.add<FLOAT>("doorin", 'd', "DoorIn", false, 0.50, cmdline::range<FLOAT>(0, 100));
	args.add<FLOAT>("bmm", '\0', "b Param in mm", false, 492.4, cmdline::range<FLOAT>(-10000, 10000));
	args.add<FLOAT>("phi", '\0', "phi Param in rad", false, 1.168, cmdline::range<FLOAT>(-10000, 10000));
	args.add<FLOAT>("uo", '\0', "uo Param in pixels", false, 1226.7, cmdline::range<FLOAT>(-10000, 10000));
	args.add<FLOAT>("vo", '\0', "vo Param in pixels", false, 1008.4, cmdline::range<FLOAT>(-10000, 10000));
	args.add<FLOAT>("fx", '\0', "fx Param in pixels", false, 2371.9, cmdline::range<FLOAT>(-10000, 10000));
	args.add<FLOAT>("fy", '\0', "fy Param in pixels", false, 2358.9, cmdline::range<FLOAT>(-10000, 10000));
	//EncoderParam
	args.add<UINT>("fcut", 'c', "FrameCut", false, 5, cmdline::range(0, 60));
	args.add<UINT>("brate", 'b', "BitRate", false, 200000000, cmdline::range(1000, 1000000000));
	args.add<UINT>("btol", 'l', "BitRateTolerance", false, 8000000, cmdline::range(4000000, 100000000));
	args.add<UINT>("ethread", '\0', "EncoderThread", false, 1, cmdline::range(1, 20));
	args.add<string>("filepath", 'p', "FilePath", false, "");
	args.add<UINT>("format", 'o', "OutputFormat", false, 1, cmdline::range(0, 1));
	//ClientParam
	args.add<UINT>("serverport", 's', "ServerPort", false, 8001, cmdline::range(0, 65535));
	args.add<string>("serveraddr", '\0', "ServerAddress", false, "127.0.0.1");

	args.parse_check(argc, argv);

	//FunctionChoice
	if (args.exist("function"))
	{
		switch (args.get<UINT>("function"))
		{
		case 1:
		{
			CalEnable = 1;
		}
		break;
		case 2:
		{
			CalEnable = 1;
			ResVisManu = 1;
		}
		break;
		case 3: {
			CalEnable = 1;
			ResVisAuto = 1;
		}
		break;
		case 4:
		{
			CalEnable = 1;
			EnableSendData = 1;
		}
		break;
		case 5:
		{
			CalEnable = 0;
			EncodeEnable = 1;
		}
		break;
		case 6:
		{
			CalEnable = 1;
			EncodeEnable = 1;
		}
		break;
		case 7:
		{
			CalEnable = 1;
			EncodeEnable = 1;
			EnableSendData = 1;
		}
		break;
		case 8:
		{
			camera.GetDevList();
			return 0;
		}
		break;
		case 9:
		{
			;
		}
		break;
		}
	}
	//CameraExposureTime
	if (args.exist("exptime"))
	{
		camerainitparam.ExposureTime = args.get<FLOAT>("exptime");
	}
	//CameraExposureAuto
	if (args.exist("expauto"))
	{
		camerainitparam.ExposureAuto = args.get<UINT>("expauto");
	}
	//CameraGain
	if (args.exist("gain"))
	{
		camerainitparam.Gain = args.get<FLOAT>("gain");
	}
	//CameraGainAuto
	if (args.exist("gainauto"))
	{
		camerainitparam.GainAuto = args.get<UINT>("gainauto");
	}
	//CameraAcquisitionFrameRate
	if (args.exist("frate"))
	{
		camerainitparam.AcquisitionFrameRate = args.get<FLOAT>("frate");
	}
	//CameraDeviceId
	if (args.exist("devid"))
	{
		camerainitparam.devNum = args.get<UINT>("devid");
	}
	//DeviceUserId
	if (args.exist("usrid"))
	{
		camerainitparam.DeviceUserId = args.get<UINT>("usrid");
	}
	//ROIHeight
	if (args.exist("roih"))
	{
		camerainitparam.ROIHeight = args.get<UINT>("roih");
	}
	//ROIWidth
	if (args.exist("roiw"))
	{
		camerainitparam.ROIWidth = args.get<UINT>("roiw");
	}
	//ROIOffsetX
	if (args.exist("roix"))
	{
		camerainitparam.ROIOffsetX = args.get<UINT>("roix");
	}
	//ROIOffsetY
	if (args.exist("roiy"))
	{
		camerainitparam.ROIOffsetY = args.get<UINT>("roiy");
	}
	//FrameSpecInfoSelector
	if (args.exist("stampsel"))
	{
		camerainitparam.FrameSpecInfoSelector = args.get<UINT>("stampsel");
	}
	//CameraFrameSpecInfo
	if (args.exist("stampon"))
	{
		camerainitparam.FrameSpecInfo = args.get<UINT>("stampon");
	}
	//CameraTriggerSelector
	if (args.exist("tsele"))
	{
		camerainitparam.TriggerSelector = args.get<UINT>("tsele");
	}
	//CameraTriggerMode
	if (args.exist("tmode"))
	{
		camerainitparam.TriggerMode = args.get<UINT>("tmode");
	}
	//CameraTriggerSource
	if (args.exist("tsrc"))
	{
		camerainitparam.TriggerSource = args.get<UINT>("tsrc");
	}
	//CameraTriggerActivation
	if (args.exist("tacti"))
	{
		camerainitparam.TriggerActivation = args.get<UINT>("tacti");
	}
	//CameraTriggerDelay
	if (args.exist("tdelay"))
	{
		camerainitparam.TriggerDelay = args.get<FLOAT>("tdelay");
	}
	//CameraTriggerCacheEnable
	if (args.exist("tcaen"))
	{
		camerainitparam.TriggerCacheEnable = args.get<UINT>("tcaen");
	}

	//GaussCal
	//MaxError
	if (args.exist("emax"))
	{
		Calparam.maxError = args.get<FLOAT>("emax");
	}
	//MinError
	if (args.exist("emin"))
	{
		Calparam.minError = args.get<FLOAT>("emin");
	}
	//XRange
	if (args.exist("xr"))
	{
		Calparam.xRange = args.get<UINT>("xr");
	}
	//GaussThread
	if (args.exist("gthread"))
	{
		Calparam.threads = args.get<UINT>("gthread");
	}
	//DoorIn
	if (args.exist("doorin")) 
	{
		IdentifyParam.doorin = args.get<FLOAT>("doorin");
	}
	//bmm
	if (args.exist("bmm"))
	{
		Calparam.b = args.get<FLOAT>("bmm");
	}
	//phi
	if (args.exist("phi"))
	{
		Calparam.phi = args.get<FLOAT>("phi");
	}
	//uo
	if (args.exist("uo"))
	{
		Calparam.uo = args.get<FLOAT>("uo");
	}
	//vo
	if (args.exist("vo"))
	{
		Calparam.vo = args.get<FLOAT>("vo");
	}
	//fx
	if (args.exist("fx"))
	{
		Calparam.fx = args.get<FLOAT>("fx");
	}
	//fy
	if (args.exist("fy"))
	{
		Calparam.fy = args.get<FLOAT>("fy");
	}


	//Encoder
	//FrameCut
	if(args.exist("fcut"))
	{
		encodeparam.FrameCut = args.get<UINT>("fcut");
	}
	//BitRate
	if (args.exist("brate"))
	{
		encoderparam.bitrate = args.get<UINT>("brate");
	}
	if (args.exist("btol"))
	{
		encoderparam.bitrate_tolerance = args.get<UINT>("btol");
	}

	//Ethread
	if (args.exist("ethread")) 
	{
		encoderparam.ethread = args.get<UINT>("ethread");
	}
	//FilePath
	if (args.exist("filepath"))
	{
		FilePath = args.get<string>("filepath").data();
	}
	//printf("PATH :%s", FilePath);
	if (args.exist("format"))
	{
		Format = args.get<UINT>("format");
	}
	//Client
	//Port
	if (args.exist("serverport"))
	{
		clientparam.StartUpPort = args.get<UINT>("serverport");
		//printf("the start port is %d", ServerStartup);
	}
	//Addr
	if (args.exist("serveraddr"))
	{
		clientparam.ServerAddr = args.get<string>("serveraddr").data();
	}

	//printf("ADDR :%s", clientparam.ServerAddr);


	//add by ylxu at 2019-1-23
	char * Filename = (char*)malloc(100);
	sprintf(Filename, "Send%d.csv", camerainitparam.devNum);
	outFile.open(Filename, ios::out);
	free(Filename);


	int ret;
	//camerainitparam.AcquisitionFrameRate = 60.0;
	ret = CameraInit(camerainitparam);
	if (ret) {
		printf("Camera Init failed\n");
	}
	
	encoderparam.in_w = camerainitparam.in_w;
	encoderparam.in_h = camerainitparam.in_h;
	//printf("%dx%d", encoderparam.in_w, encoderparam.in_h);

	FinalPath = (char*)malloc(500);

	ret = CreateFolder(FilePath, FinalPath, camerainitparam.devNum);
	if (!ret)
	{
		printf("Create Folder Failed\n");
	}
	encoderparam.filepath = FinalPath;
	encodeparam.filepath = FinalPath;

	encoderparam.CameraNum = camerainitparam.devNum;
	encodeparam.CameraNum = camerainitparam.devNum;

	if (Format == 0)
	{
		EncoderMJPEGInit(encoderparam);
	}
	else if (Format == 1)
	{
		EncoderJPEGInit(encoderparam);
	}
	
	//printf("Please input the thread num:");
	//scanf("%d", &Calparam.threads);
	//printf("Input 1 to Transport the data to server:");
	//scanf("%d", &EnableSendData);
	if (EnableSendData) {
		ClientInit();
	}
	
	

	//Encodeparam
	encodeparam.nHeight = ImageHeight;
	encodeparam.nWidth = ImageWidth;


	//AcqImageThread();
	//watch.start();
	SetConsoleCtrlHandler(ConsoleHandler, TRUE);

	thread acqthread(AcqImageThread);

	if (CalEnable&&!EncodeEnable) {
		thread calthread(CalImageThread);
		if (!ResVisAuto && !ResVisManu) {
			thread TimerThread(TimerPerformance);
			TimerThread.join();
		}
		calthread.join();
	}

	//thread SendClient(SendResToPort);
	if (!CalEnable&&EncodeEnable) {
		if (Format == 0)
		{
			thread EncoderThread(EncodeMJPEGThread);
			thread TimerThread(TimerPerformance);
			TimerThread.join();
			EncoderThread.join();
		}
		else if (Format == 1)
		{
			thread EncoderThread(EncodeJPEGThread);
			thread TimerThread(TimerPerformance);
			TimerThread.join();
			EncoderThread.join();
		}
		
	}

	if (CalEnable&&EncodeEnable) {
		if (Format == 0)
		{
			thread EncoderThread(EncodeMJPEGThread);
			thread calthread(CalImageThread);
			thread TimerThread(TimerPerformance);
			TimerThread.join();
			EncoderThread.join();
			calthread.join();
		}
		else if (Format == 1)
		{
			thread EncoderThread(EncodeJPEGThread);
			thread calthread(CalImageThread);
			thread TimerThread(TimerPerformance);
			TimerThread.join();
			EncoderThread.join();
			calthread.join();
		}
		
	}

	acqthread.join();

	//SendClient.join();
	printf("Stop Grabing����\n");
	ret = CameraClean();
	if (!ret) {
		printf("Camera Clean failed\n");
	}
	
	if (Format == 0)
	{
		printf("Stop Encoding......\n");
		EncoderMJPEGClean();
	}
	else if (Format == 1)
	{
		printf("Stop Encoding......\n");
		EncoderJPEGClean();
	}
	
	ClientClean();

	free(FinalPath);

	printf("Application Accomplished!\n\n");

	return 0;
}