#pragma once
extern "C"

{

#include "libavcodec/avcodec.h"

#include "libavformat/avformat.h"

#include "libavformat/avio.h"

};

typedef struct {
	//YUV's width and height
	int in_w;
	int in_h;
	//BitRate
	int bitrate = 4000000;
	//EncoderTHread
	int ethread = 2;
}EncoderParam;

typedef struct {
	//YUV's width and height
	int nWidth;
	int nHeight;

	//ImageBuffer
	unsigned char* pBufAddr;
	
	//Timestamp
	int pts;
	//CameraNum
	int CameraNum = 0;

	//FrameCut
	int FrameCut = 5;//Encode 1 frame before every FrameCut frame

	const char* filepath;

}EncodeParam;

class Encoder {
public:
	//Encode Initiate
	int Init(EncoderParam encoderparam);

	//Encode Image
	int Encode(EncodeParam encodeparam);

	//Clean Context
	int Clean();

	//Flush Encoder
	int Flush(); //to be continued

private:
	AVFormatContext* pFormatCtx;

	AVOutputFormat* fmt;

	AVStream* video_st;

	AVCodecContext* pCodecCtx;

	AVCodec* pCodec;

	uint8_t* picture_buf;

	AVFrame* picture;

	AVPacket pkt;

	uint8_t *pDataForYUV = NULL;

	AVCodecID codec_id = AV_CODEC_ID_MJPEG;

	AVDictionary *param = 0;

	int y_size;

	int got_picture = 0;

	int size;

	//int framecount = 100;

	int ret = 0;

	//int i = 0;

	//FILE *in_file = NULL;                            //YUV source

	int in_w, in_h;       //YUV's width and height

	//int CameraNum = 0;
};