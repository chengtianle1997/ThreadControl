#pragma once
#include "iostream"
#include "Timer.h"
#include "CameraSDK.h"
#include "FFmpegEncode.h"
#include "GaussCal.h"
#include "ClientNet.h"
#include <thread>
#include <mutex>
#include <stdlib.h>
#include <queue>

using namespace std;

using namespace cv;

stop_watch watch;

Camera camera;

Encoder encoder;

GaussCal Gausscal;

ClientNet client;

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
MV_FRAME_OUT_INFO_EX Buffer0Info;
MV_FRAME_OUT_INFO_EX Buffer1Info;

int SizeofPixels = 0;
int ImageWidth = 0;
int ImageHeight = 0;

//The Exit Instruction(Set 1 to Stop)
int AcqExit = 0;
int CalExit = 0;
int EncodeExit = 0;
int SendExit = 0;

//Functional choice
int ResVisAuto = 0;
int ResVisManu = 0;

int PerforFramecnt = 0;

//The Server Information
int ServerPort = 8888;
const char* ServerAddr = "127.0.0.1";

//The Socket Package
//SocketPackage *Sockpack = { 0 };
//The sending queue

static queue<SocketPackage> SendQueue;

//The Application Entrance
int DeviceNum = 0;
