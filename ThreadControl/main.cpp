
#define _CRT_SECURE_NO_WARNINGS 

//#define ENCODE_ENABLE

#include "main.h"

ofstream outFile;


int CameraInit(CameraInitParam &camerainitparam)
{
	int ret;

	ret = camera.GetDevList(camerainitparam);
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

void CsClinetInit() {
	int ret;

	printf("The CsClient is connecting.....\n");

	//int Serverport;

	Serverport = csclientparam.StartUpPort + camerainitparam.devNum;

	printf("ServerPort:%d\n", Serverport);

	ret = client.CsClientConnect(Serverport, csclientparam.ServerAddr);

	if (ret)
	{
		printf("Cs Connecting failed!");
	}
}

void CsClientClean() {
	printf("Closing CsSocket......\n");
	client.CsClientClose();
}

void DsClientInit() {
	int ret;

	printf("The DsClient is connecting.....\n");

	//int Serverport;

	Serverport = dsclientparam.StartUpPort + camerainitparam.devNum;

	ret = client.DsClientConnect(Serverport, dsclientparam.ServerAddr);

	if (ret)
	{
		printf("Ds Connecting failed!");
	}
}

void DsClientClean() {
	printf("Closing DsSocket......\n");
	client.DsClientClose();
}


void ResizeAndSendPic() {
	if (EnableSendData) {
		if (CodeState0 && !Buffer0Mutex)
		{
			Mat src{
			cvSize(ImageWidth,ImageHeight),
			CV_8UC1,
			Buffer0
			};
			Mat image0;
			resize(src, image0, Size(PictureWidth, PictureHeight));
			//imshow("1", image0);
			//cvWaitKey(0);
			SocketPicture Sockpic;
			//数据入包
			memcpy(Sockpic.SerialNum ,camerainitparam.SerialNum,64);
			Sockpic.Framecnt = encodeparam.pts;
			for (int i = 0; i < PictureHeight; i++)
			{
				uchar* data = image0.ptr<uchar>(i);
				for (int j = 0; j < PictureWidth; j++)
				{
					Sockpic.Picture[i][j] = data[j];
				}
			}
			//数据转换
			char* picturebuffer = (char*)malloc(sizeof(SocketPicture));
			memcpy(picturebuffer, &Sockpic, sizeof(SocketPicture));

			//发包
			client.CsClientSend(picturebuffer, sizeof(SocketPicture));

		}
		else if (CodeState1 && !Buffer1Mutex)
		{
			Mat src{
			cvSize(ImageWidth,ImageHeight),
			CV_8UC1,
			Buffer1
			};
			Mat image1;
			resize(src, image1, Size(PictureWidth, PictureHeight));
			SocketPicture Sockpic;
			//数据入包
			memcpy(Sockpic.SerialNum, camerainitparam.SerialNum, 64);
			Sockpic.Framecnt = encodeparam.pts;
			for (int i = 0; i < PictureHeight; i++)
			{
				uchar* data = image1.ptr<uchar>(i);
				for (int j = 0; j < PictureWidth; j++)
				{
					Sockpic.Picture[i][j] = data[j];
				}
			}
			//数据转换
			char* picturebuffer = (char*)malloc(sizeof(SocketPicture));
			memcpy(picturebuffer, &Sockpic, sizeof(SocketPicture));

			//发包
			client.CsClientSend(picturebuffer, sizeof(SocketPicture));

		}
	}

	

}

void SendDataToCsServer() {
	if (GetImage0 && !CalEnd0 && !Buffer0Mutex) {
		//Package the Data
		SocketPackageTrim PackageData0;
		for (int i = 0; i < INFO_MAX_BUFFER_SIZE; i++)
		{
			PackageData0.SerialNumber[i] = camerainitparam.DevInfo.SpecialInfo.stUsb3VInfo.chSerialNumber[i];
		}

		PackageData0.Framecnt = Buffer0Info.nFrameNum;
		for (int i = 0; i < DataRows/DataTrim; i++)
		{
			PackageData0.s[i] = Calparam.point[i*10].s;
			PackageData0.ay[i] = Calparam.point[i*10].ay;
		}

		//outFile << PackageData0.SerialNumber << "," << PackageData0.Framecnt;
		//outFile << endl;

		//SendQueue.push(PackageData0);
		//int q = SendQueue.size();
		if (EnableSendData)
		{
			char* buf = (char*)malloc(sizeof(SocketPackageTrim));

			memcpy(buf, &PackageData0, sizeof(SocketPackageTrim));

			client.DsClientSend(buf, sizeof(SocketPackageTrim));

			free(buf);
		}
	}

	else if (GetImage1 && !CalEnd1 && !Buffer1Mutex)
	{
		//Package the Data
		SocketPackageTrim PackageData1;
		for (int i = 0; i < INFO_MAX_BUFFER_SIZE; i++)
		{
			PackageData1.SerialNumber[i] = camerainitparam.DevInfo.SpecialInfo.stUsb3VInfo.chSerialNumber[i];
		}

		PackageData1.Framecnt = Buffer0Info.nFrameNum;
		for (int i = 0; i < DataRows / DataTrim; i++)
		{
			PackageData1.s[i] = Calparam.point[i*10].s;
			PackageData1.ay[i] = Calparam.point[i*10].ay;
		}

		//outFile << PackageData1.SerialNumber << "," << PackageData1.Framecnt;
		//outFile << endl;

		//SendQueue.push(PackageData1);
		if (EnableSendData)
		{
			char* buf = (char*)malloc(sizeof(SocketPackageTrim));

			memcpy(buf, &PackageData1, sizeof(SocketPackageTrim));

			client.DsClientSend(buf, sizeof(SocketPackageTrim));


		}
	}

}

//void SendDataToCsServer() {
//	if (GetImage0 && !CalEnd0 && !Buffer0Mutex) {
//		//Package the Data
//		//SocketPackageTrim PackageData0;
//		SocketPackage PackageData0;
//		for (int i = 0; i < INFO_MAX_BUFFER_SIZE; i++)
//		{
//			PackageData0.SerialNumber[i] = camerainitparam.DevInfo.SpecialInfo.stUsb3VInfo.chSerialNumber[i];
//		}
//
//		PackageData0.Framecnt = Buffer0Info.nFrameNum;
//		//for (int i = 0; i < DataRows/DataTrim; i++)
//		for (int i = 0; i < DataRows; i++)
//		{
//			/*PackageData0.s[i] = Calparam.point[i*DataTrim].s;
//			PackageData0.ay[i] = Calparam.point[i*DataTrim].ay;*/
//			PackageData0.s[i] = Calparam.point[i].s;
//			PackageData0.ay[i] = Calparam.point[i].ay;
//		}
//
//		//outFile << PackageData0.SerialNumber << "," << PackageData0.Framecnt;
//		//outFile << endl;
//
//		//SendQueue.push(PackageData0);
//		//int q = SendQueue.size();
//		if (EnableSendData)
//		{
//			/*char* buf = (char*)malloc(sizeof(SocketPackageTrim));
//
//			memcpy(buf, &PackageData0, sizeof(SocketPackageTrim));
//
//			client.DsClientSend(buf, sizeof(SocketPackageTrim));
//
//			free(buf);*/
//			char* buf = (char*)malloc(sizeof(SocketPackage));
//
//			memcpy(buf, &PackageData0, sizeof(SocketPackage));
//
//			client.DsClientSend(buf, sizeof(SocketPackage));
//
//			free(buf);
//
//		}
//	}
//
//	else if (GetImage1 && !CalEnd1 && !Buffer1Mutex)
//	{
//		//Package the Data
//		//SocketPackageTrim PackageData1;
//		SocketPackage PackageData1;
//		for (int i = 0; i < INFO_MAX_BUFFER_SIZE; i++)
//		{
//			PackageData1.SerialNumber[i] = camerainitparam.DevInfo.SpecialInfo.stUsb3VInfo.chSerialNumber[i];
//		}
//
//		PackageData1.Framecnt = Buffer0Info.nFrameNum;
//		//for (int i = 0; i < DataRows / DataTrim; i++)
//		for (int i = 0; i < DataRows; i++)
//		{
//			/*PackageData1.s[i] = Calparam.point[i*DataTrim].s;
//			PackageData1.ay[i] = Calparam.point[i*DataTrim].ay;*/
//			PackageData1.s[i] = Calparam.point[i].s;
//			PackageData1.ay[i] = Calparam.point[i].ay;
//		}
//
//		//outFile << PackageData1.SerialNumber << "," << PackageData1.Framecnt;
//		//outFile << endl;
//
//		//SendQueue.push(PackageData1);
//		if (EnableSendData)
//		{
//			/*char* buf = (char*)malloc(sizeof(SocketPackageTrim));
//
//			memcpy(buf, &PackageData1, sizeof(SocketPackageTrim));
//
//			client.DsClientSend(buf, sizeof(SocketPackageTrim));*/
//			char* buf = (char*)malloc(sizeof(SocketPackage));
//
//			memcpy(buf, &PackageData1, sizeof(SocketPackage));
//
//			client.DsClientSend(buf, sizeof(SocketPackage));
//
//		}
//	}
//
//}


int CreateFolder(const char* filepath, char* finalpath, unsigned char* SerialNum)
{
	int nRet;
	nRet = filemanager.CreateFolder(filepath, finalpath, SerialNum);
	
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
			if (EnableCSSend) {
				if (PerforFramecntAll) {
					if (PerforFramecntAll >= FrameRateControlData) {
						if ((Buffer0Info.nFrameNum % ((PerforFramecntAll / FrameRateControlData))) == 0) {
							thread SendToCsServer0(SendDataToCsServer);
							SendToCsServer0.detach();
						}
					}
					else if (PerforFramecntAll < FrameRateControlData) {
						thread SendToCsServer0(SendDataToCsServer);
						SendToCsServer0.detach();
					}
				}
			}
			
			if (EnableSendData)
			{
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

				//outFile << PackageData0.SerialNumber << "," << PackageData0.Framecnt;
				//outFile << endl;

				//SendQueue.push(PackageData0);
				//int q = SendQueue.size();
				if (EnableSendData)
				{
					char* buf = (char*)malloc(sizeof(SocketPackage));

					memcpy(buf, &PackageData0, sizeof(SocketPackage));

					client.ClientSend(buf, sizeof(SocketPackage));

					free(buf);


				}
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

			if (EnableCSSend) {
				if (PerforFramecntAll) {
					if (PerforFramecntAll >= FrameRateControlData)
					{
						if ((Buffer1Info.nFrameNum % ((PerforFramecntAll / FrameRateControlData))) == 0)
						{
							thread SendToCsServer1(SendDataToCsServer);
							SendToCsServer1.detach();
						}
					}
					else if (PerforFramecntAll < FrameRateControlData)
					{
						thread SendToCsServer1(SendDataToCsServer);
						SendToCsServer1.detach();
					}
				}
			}
			
			
			if (EnableSendData) 
			{
				SocketPackage PackageData1;
				for (int i = 0; i < INFO_MAX_BUFFER_SIZE; i++)
				{
					PackageData1.SerialNumber[i] = camerainitparam.DevInfo.SpecialInfo.stUsb3VInfo.chSerialNumber[i];
				}

				PackageData1.Framecnt = Buffer1Info.nFrameNum;
				for (int i = 0; i < ImageHeight; i++)
				{
					PackageData1.s[i] = Calparam.point[i].s;
					PackageData1.ay[i] = Calparam.point[i].ay;
				}

				//outFile << PackageData1.SerialNumber << "," << PackageData1.Framecnt;
				//outFile << endl;

				//SendQueue.push(PackageData1);
				if (EnableSendData)
				{
					char* buf = (char*)malloc(sizeof(SocketPackage));

					memcpy(buf, &PackageData1, sizeof(SocketPackage));

					client.ClientSend(buf, sizeof(SocketPackage));


				}
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

	//定义变量
	int dark[9000];     //黑图的编号
	int bright[4000];   //白图的编号
	//string ADD;              //获取地址时使用
	int i(1), j(0), k(0);      //循环和计数变量
	float diff = 0.0;          //相邻两图片的亮度差值
	int Refer = 0;
	int Refer2 = 0;
	int Refer3 = 0;          //亮度参考值
	float MeanDev = 0.0;     //平均偏差
	float Mean = 0.0;        //平均值
	int num;                 //文件夹中图片的数量
	int Minchange = 40;           //亮度变化多大时重新取样

	int Analyzingcounter = 0;
	float AverageofAverage = 0;
	float *AverageData = (float*)malloc(encodeparam.FrameCut+1);
	float MaxAvr = 0;
	float MaxVeAvr = 0;
	int MaxPos = 0;
	int MaxVerify = 0;
	int FindBeatTimes = 0;
	

	while (!AcqExit) {

		//stOutFrame = (MV_FRAME_OUT*)malloc(sizeof(MV_FRAME_OUT));
		int ret = camera.GetImage(stOutFrame);
		if (ret == MV_OK) {
			//Analyze The photo
			if ((Analyzingcounter < (encodeparam.FrameCut + 1)) && !AnalyzeEnd )
			{
				Mat matImage{
				cvSize(ImageWidth,ImageHeight),
				CV_8UC1,
				(stOutFrame).pBufAddr
				};

				Rect rect(0,0,camerainitparam.in_w,500);

				Mat RoiImage = matImage(rect);

				cv::Scalar AverPic = cv::mean(RoiImage);

				//AverageData[stOutFrame.stFrameInfo.nFrameNum% (encodeparam.FrameCut + 1)] =  AverPic[0];

				//AverageofAverage = (AverageofAverage + AverageData[stOutFrame.stFrameInfo.nFrameNum % (encodeparam.FrameCut + 1)]) / 2;
				
				if (AverPic[0] > MaxAvr)
				{
					MaxAvr = AverPic[0];
					MaxPos = stOutFrame.stFrameInfo.nFrameNum % (encodeparam.FrameCut + 1);

				}				
				Analyzingcounter++;


			}
			if ( Analyzingcounter >= (encodeparam.FrameCut + 1) && !AnalyzeEnd )
			{
				Mat matImage{
				cvSize(ImageWidth,ImageHeight),
				CV_8UC1,
				(stOutFrame).pBufAddr
				};

				Rect rect(0, 0, camerainitparam.in_w, 500);

				Mat RoiImage = matImage(rect);

				cv::Scalar AverPic = cv::mean(RoiImage);

				//cv::Scalar AverPic = cv::mean(matImage);

				if (AverPic[0] > MaxVeAvr)
				{
					MaxVeAvr = AverPic[0];
					MaxVerify = stOutFrame.stFrameInfo.nFrameNum % (encodeparam.FrameCut + 1);

				}

				if (Analyzingcounter == 2 * (encodeparam.FrameCut + 1) - 1)
				{
					//Get the right beat
					if (MaxVerify == MaxPos) {
						AnalyzeEnd = true;
						BeatRecord = MaxVerify;
						Analyzingcounter = 0;
						MaxVerify = 0;
						MaxAvr = 0;
						FindBeatTimes = 0;
					}
					//Get The Wrong one
					else {
						Analyzingcounter = 0;
						MaxVerify = 0;
						MaxAvr = 0;
						FindBeatTimes++;
					}
				}

				Analyzingcounter++;

				//Get Beat Time out
				if (FindBeatTimes > 30) {
					AnalyzeEnd = true;
					printf("\nCan't find the beat!\n\n");
					FindBeatTimes = 0;
					Analyzingcounter = 0;
					MaxVerify = 0;
					MaxAvr = 0;
				}

			}

			//Decide the Buffer 
			lock_guard<mutex> lockGuard(m);
			if (CalEnd0)
			{
				Buffer0Mutex = 1;
				memcpy(Buffer0, (stOutFrame).pBufAddr, SizeofPixels);
				Buffer0Mutex = 0;
				Buffer0Info = stOutFrame.stFrameInfo;
				DetectState0 = 1;
				Framenum = stOutFrame.stFrameInfo.nFrameNum;
				//printf("Framenum:%d",Framenum);
				//Classify the picture according to the beat
				if (Framenum % (encodeparam.FrameCut + 1) == BeatRecord) 
				{
					if (EncodeEnable) 
					{
						CodeState0 = 1;
						GetImage0 = 0;
						CalEnd0 = 1;
					}

				}
				else
				{
					if (CalEnable) {
						CalEnd0 = 0;
						GetImage0 = 1;
					}
				}


			}
			else if (CalEnd1)
			{
				Buffer1Mutex = 1;
				memcpy(Buffer1, (stOutFrame).pBufAddr, SizeofPixels);
				Buffer1Mutex = 0;
				Buffer1Info = stOutFrame.stFrameInfo;
				DetectState1 = 1;
				Framenum = stOutFrame.stFrameInfo.nFrameNum;
				//printf("Framenum:%d",Framenum);
				if (Framenum % (encodeparam.FrameCut + 1) == BeatRecord)
				{
					if (EncodeEnable)
					{
						CodeState1 = 1;
						GetImage1 = 0;
						CalEnd1 = 1;
					}
				}
				else
				{
					if (CalEnable) {
						CalEnd1 = 0;
						GetImage1 = 1;
					}
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

			if (EnableCSSend) {
				if (PerforFrameencAll)
				{
					if (PerforFrameencAll >= FrameRateControlPic)
					{
						if ((pts % ((PerforFrameencAll / FrameRateControlPic))) == 0) {
							thread SendBuffer0(ResizeAndSendPic);
							SendBuffer0.detach();
						}
					}
					else if (PerforFrameencAll < FrameRateControlPic)
					{
						thread SendBuffer0(ResizeAndSendPic);
						SendBuffer0.detach();
					}
				}
			}
			
			
			
			encoder.EncodeMJPEG(encodeparam);
			pts++;
			CodeState0 = 0;
			PerforFrameenc++;	
			//SendBuffer0.join();
			
		}
		else if (CodeState1 && !Buffer1Mutex)
		{
			encodeparam.pBufAddr = Buffer1;
			encodeparam.pts = pts;

			if (EnableCSSend)
			{
				if (PerforFrameencAll)
				{
					if (PerforFrameencAll >= FrameRateControlPic)
					{
						if ((pts % ((PerforFrameencAll / FrameRateControlPic))) == 0) {
							thread SendBuffer1(ResizeAndSendPic);
							SendBuffer1.detach();
						}
					}
					else if (PerforFrameencAll < FrameRateControlPic)
					{
						thread SendBuffer1(ResizeAndSendPic);
						SendBuffer1.detach();
					}
				}
			}
			
			
			encoder.EncodeMJPEG(encodeparam);
			pts++;
			CodeState1 = 0;
			PerforFrameenc++;
			//SendBuffer1.join();
			
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
				if (IsFirst)
				{
					//Record The FrameRate
					PerforFramecntAll = PerforFramecnt;
					IsFirst = false;
				}
				
				else if (!IsFirst)
				{
					//Record The Average FrameRate
					PerforFramecntAll = (PerforFramecnt + PerforFramecntAll) / 2;
				}
				
			}
			else if (!CalEnable&&EncodeEnable) {
				printf("\n\n\n\nThe Encode Framerate is %d fps\n\n\n\n", PerforFrameenc);
				if (IsFirst)
				{
					//Record The FrameRate
					PerforFrameencAll = PerforFrameenc;
					IsFirst = false;
				}

				else if (!IsFirst)
				{
					//Record The Average FrameRate
					PerforFrameencAll = (PerforFrameenc + PerforFrameencAll) / 2;
				}
				
			}
			else if (CalEnable&&EncodeEnable) {
				printf("\n\n\n\n\nThe Calculate Framerate is %d fps, \t The Encode Framerate is %d fps\n\n\n\n\n\n", PerforFramecnt,PerforFrameenc);
				if (IsFirst)
				{

					//Record The FrameRate
					PerforFramecntAll = PerforFramecnt;
					PerforFrameencAll = PerforFrameenc;
					IsFirst = false;
				}
				else if (!IsFirst)
				{
					//Record The Average FrameRate
					PerforFramecntAll = (PerforFramecnt + PerforFramecntAll) / 2;
					PerforFrameencAll = (PerforFrameenc + PerforFrameencAll) / 2;
				}
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

//Thread to Detect the unnormal consequeces
void DetectThread() {
	//The Debug Log to Check the result
	//char * Filename = (char*)malloc(100);
	//sprintf(Filename, "Detect%s.csv", camerainitparam.SerialNum);
	//outFile.open(Filename, ios::out);
	//free(Filename);
	while (!DetectExit)
	{
		if (CodeState0 || GetImage0)
		{
			//if had not detected
			if (DetectState0) {
				//printf("");
				if (Buffer0Info.nFrameNum > MaxCnt)
				{
					MaxCnt = Buffer0Info.nFrameNum;
					//outFile << MaxCnt << "," << BeatRecord << endl;
				}
				else if (Buffer0Info.nFrameNum < MaxCnt)
				{
					//BeatRecord = ((BeatRecord - MaxCnt - 1) % (encodeparam.FrameCut + 1)+ (encodeparam.FrameCut + 1)*2 -5 )% (encodeparam.FrameCut + 1);
					AnalyzeEnd = false;
					printf("BeatRecord Change");
					MaxCnt = Buffer0Info.nFrameNum;
					//outFile << MaxCnt <<","<< BeatRecord <<endl;
				}
				DetectState0 = 0;
			}
			
		}
		else if (CodeState1 || GetImage1)
		{
			//printf("");
			//if had not detected
			if (DetectState1) {
				if (Buffer1Info.nFrameNum > MaxCnt)
				{
					MaxCnt = Buffer1Info.nFrameNum;
					//outFile << MaxCnt << "," << BeatRecord << endl;
				}
				else if (Buffer1Info.nFrameNum < MaxCnt)
				{
					//BeatRecord = ((BeatRecord - MaxCnt - 1) % (encodeparam.FrameCut + 1) + (encodeparam.FrameCut + 1)*2-5 ) % (encodeparam.FrameCut + 1);
					printf("BeatRecord Change");
					AnalyzeEnd = false;
					MaxCnt = Buffer1Info.nFrameNum;
					outFile << MaxCnt << "," << BeatRecord << endl;
				}
				DetectState1 = 0;
			}
			
			
		}
		
	}
	outFile.close();
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

//程序停止指令监听
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
		DetectExit = 1;
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
		DetectExit = 1;

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
	args.add<UINT>("function", 'f', "FunctionChoice", false, 1, cmdline::range(1, 10));
	//CameraParam
	args.add<FLOAT>("exptime", 'e', "CameraExposureTime", false, 13000, cmdline::range<FLOAT>(0, 1000000));
	args.add<UINT>("expauto", '\0', "CameraExposureAuto", false, 0, cmdline::range(0, 2));
	args.add<FLOAT>("gain", 'g', "CameraGain", false, 15, cmdline::range<FLOAT>(0, 15));
	args.add<UINT>("gainauto", '\0', "CameraGainAuto", false, 0, cmdline::range(0, 2));
	args.add<FLOAT>("frate", 'r', "CameraAcquisitionFrameRate", false, 60, cmdline::range<FLOAT>(0, 200));
	args.add<string>("devsr", 'v', "CameraDeviceSerialNum", false, "");
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
	args.add<FLOAT>("kx", '\0', "kx Param for Angular Correction", false, 0.9558, cmdline::range<FLOAT>(-10000, 10000));
	args.add<FLOAT>("ky", '\0', "ky Param for Angular Correction", false, 0.9558, cmdline::range<FLOAT>(-10000, 10000));
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
			camera.GetDevList(camerainitparam);
			return 0;
		}
		break;
		case 9:
		{
			;
		}
		break;
		case 10:
		{
			CalEnable = 1;
			EncodeEnable = 1;
			EnableSendData = 1;
			EnableCSSend = 1;
		}
		break;
		default:
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
	if (args.exist("devsr"))
	{
		SrNum = args.get<string>("devsr").data();
		int srlen = 0;
		srlen = strlen(SrNum);
		for (int i = 0; i < srlen; i++) {
			camerainitparam.SerialNum[i] = (unsigned char)SrNum[i];
		}
		
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
	//kx
	if (args.exist("kx"))
	{
		Calparam.kx = args.get<FLOAT>("kx");
	}
	//ky
	if (args.exist("ky"))
	{
		Calparam.ky = args.get<FLOAT>("ky");
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
		csclientparam.ServerAddr = args.get<string>("serveraddr").data();
		dsclientparam.ServerAddr = args.get<string>("serveraddr").data();
	}

	//printf("ADDR :%s", clientparam.ServerAddr);


	//add by ylxu at 2019-1-23
	/*char * Filename = (char*)malloc(100);
	sprintf(Filename, "Send%d.csv", camerainitparam.devNum);
	outFile.open(Filename, ios::out);
	free(Filename);
*/

	//Test Camera 
	//const char *CST = "00660023843";
	//memcpy(camerainitparam.SerialNum, CST, sizeof(camerainitparam.SerialNum));

	int ret;
	//camerainitparam.AcquisitionFrameRate = 60.0;
	//camerainitparam.devNum = 0;
	ret = CameraInit(camerainitparam);
	if (ret) {
		printf("Camera Init failed\n");
		return -1;
	}
	
	encoderparam.in_w = camerainitparam.in_w;
	encoderparam.in_h = camerainitparam.in_h;
	//printf("%dx%d", encoderparam.in_w, encoderparam.in_h);

	FinalPath = (char*)malloc(500);

	ret = CreateFolder(FilePath, FinalPath, camerainitparam.SerialNum);
	if (!ret)
	{
		printf("Create Folder Failed\n");
	}
	encoderparam.filepath = FinalPath;
	encodeparam.filepath = FinalPath;

	strcpy((char*)encoderparam.SerialNum,(char*)camerainitparam.SerialNum);

	//encoderparam.CameraNum = camerainitparam.devNum;
	encodeparam.CameraNum = camerainitparam.devNum;

	if (Format == 0)
	{
		EncoderMJPEGInit(encoderparam);
		//CsClinetInit();
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
		//CsClinetInit();
		//DsClientInit();
	}

	if (EnableCSSend)
	{
		CsClinetInit();
		DsClientInit();
	}
	
	
	//Encodeparam
	encodeparam.nHeight = ImageHeight;
	encodeparam.nWidth = ImageWidth;


	//AcqImageThread();
	//watch.start();

	//放置 程序关闭 监听
	SetConsoleCtrlHandler(ConsoleHandler, TRUE);
	thread detecthread(DetectThread);
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
	detecthread.join();

	//SendClient.join();
	printf("Stop Grabing……\n");
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
	if (EnableSendData) {
		ClientClean();
		//CsClientClean();
		//DsClientClean();
	}
	//ClientClean();

	if (EnableCSSend)
	{
		CsClientClean();
		DsClientClean();
	}
	

	free(FinalPath);

	printf("Application Accomplished!\n\n");

	return 0;
}