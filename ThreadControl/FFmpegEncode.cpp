#include "FFmpegEncode.h"

int Encoder::Init(EncoderParam encoderparam)
{
	//Encode with FFmpeg
	in_w = encoderparam.in_w;

	in_h = encoderparam.in_h;

	av_register_all();

	pFormatCtx = avformat_alloc_context();

	//Guess format
	fmt = av_guess_format("mjpeg", NULL, NULL);

	pFormatCtx->oformat = fmt;

	video_st = avformat_new_stream(pFormatCtx, 0);
	if (video_st == NULL) {
		return -1;
	}

	pCodecCtx = video_st->codec;

	pCodecCtx->codec_id = fmt->video_codec;

	pCodecCtx->codec_type = AVMEDIA_TYPE_VIDEO;

	pCodecCtx->pix_fmt = AV_PIX_FMT_YUVJ444P;



	pCodecCtx->width = in_w;

	pCodecCtx->height = in_h;

	pCodecCtx->bit_rate = encoderparam.bitrate;

	pCodecCtx->time_base.num = 1;

	pCodecCtx->time_base.den = 25;

	pCodecCtx->framerate.num = 25;

	pCodecCtx->framerate.den = 1;

	pCodecCtx->thread_count = encoderparam.ethread;

	av_dict_set(&param, "preset", "slow", 0);

	av_dict_set(&param, "tune", "zerolatecy", 0);


	//Output some information

	//av_dump_format(pFormatCtx, 0, out_file, 1);



	pCodec = avcodec_find_encoder(pCodecCtx->codec_id);

	if (!pCodec) {

		printf("Codec not found.");

		return -1;

	}

	if (avcodec_open2(pCodecCtx, pCodec, NULL) < 0) {

		printf("Could not open codec.");

		return -1;

	}

	pCodec->capabilities = AV_CODEC_CAP_SLICE_THREADS;

	avcodec_parameters_from_context(video_st->codecpar, pCodecCtx);


	picture = av_frame_alloc();

	size = avpicture_get_size(pCodecCtx->pix_fmt, pCodecCtx->width, pCodecCtx->height);

	picture->width = pCodecCtx->width;

	picture->height = pCodecCtx->height;

	picture->format = pCodecCtx->pix_fmt;

	picture_buf = (uint8_t *)av_malloc(size);

	if (!picture_buf)

	{

		return -1;

	}

	avpicture_fill((AVPicture *)picture, picture_buf, pCodecCtx->pix_fmt, pCodecCtx->width, pCodecCtx->height);

	//Write Header

	//avformat_write_header(pFormatCtx, NULL);

	y_size = pCodecCtx->width * pCodecCtx->height;
}

int Encoder::Encode(EncodeParam encodeparam)
{
	char* out_file = 0;

	out_file = (char*)malloc(40);

	//Êä³öÎÄ¼þ
	sprintf(out_file, "Camera%d_%d.jpeg", encodeparam.CameraNum, encodeparam.pts);

	//Output some information
	av_dump_format(pFormatCtx, 0, out_file, 1);

	//Output URL
	if (avio_open(&pFormatCtx->pb, out_file, AVIO_FLAG_READ_WRITE) < 0) {
		printf("Couldn't open output file.");
		return -1;
	}

	avformat_init_output(pFormatCtx, &param);

	//Write Header

	avformat_write_header(pFormatCtx, &param);

	//YUV420 Planar
	pDataForYUV = (uint8_t*)malloc(encodeparam.nWidth * encodeparam.nHeight * 3);
	if (NULL == pDataForYUV)
	{
		printf("malloc pDataForYUV420 fail !\n");
		//break;
	}

	av_new_packet(&pkt, y_size * 3);

	//printf("%d",sizeof(stOutFrame.pBufAddr));

	//Read YUV

	//memcpy(picture_buf, stOutFrame.pBufAddr, stOutFrame.stFrameInfo.nHeight*stOutFrame.stFrameInfo.nWidth);

	//picture->data[0] = picture_buf;              // Y

	//picture->data[1] = picture_buf + y_size;      // U 

	//picture->data[2] = picture_buf + y_size * 5 / 4;  // V


	for (int j = 0; j < encodeparam.nWidth * encodeparam.nHeight; j++)
	{
		pDataForYUV[j] = (uint8_t)encodeparam.pBufAddr[j];
		pDataForYUV[encodeparam.nWidth * encodeparam.nHeight + j] = 128;
		pDataForYUV[encodeparam.nWidth * encodeparam.nHeight * 2 + j] = 128;

	}

	picture->data[0] = pDataForYUV;

	picture->data[1] = pDataForYUV + y_size;

	picture->data[2] = pDataForYUV + y_size * 2;

	picture->pts = encodeparam.pts;

	//Encode

	ret = avcodec_encode_video2(pCodecCtx, &pkt, picture, &got_picture);

	if (ret < 0) {

		printf("Encode Error.\n");

		return -1;

	}

	if (got_picture == 1) {

		pkt.stream_index = video_st->index;

		ret = av_write_frame(pFormatCtx, &pkt);

	}

	//Write Trailer
	av_write_trailer(pFormatCtx);

	//i++;

	av_free_packet(&pkt);

	free(pDataForYUV);

	free(out_file);
}

int Encoder::Clean()
{
	printf("Encode Successful.\n");



	if (video_st) {

		avcodec_close(video_st->codec);

		av_free(picture);

		av_free(picture_buf);

	}

	avio_close(pFormatCtx->pb);

	avformat_free_context(pFormatCtx);

	return 0;

}

