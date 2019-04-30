#pragma once

#include<stdio.h>

#include<windows.h>

#define PictureHeight 205
#define PictureWidth 259
#define DataRows 2048
#define DataTrim 10

#pragma comment(lib, "Ws2_32.lib")

typedef struct {

	unsigned char SerialNumber[64]; 
	int Framecnt;
	float s[DataRows];
	float ay[DataRows];

}SocketPackage;

typedef struct {
	unsigned char SerialNumber[64];
	int Framecnt;
	float s[DataRows/DataTrim];
	float ay[DataRows/DataTrim];
}SocketPackageTrim;

typedef struct {
	char SerialNum[64];
	int Framecnt;
	int Picture[PictureHeight][PictureWidth];
}SocketPicture;

typedef struct {
	int StartUpPort = 8001;
	const char* ServerAddr = "127.0.0.1";
}ClientParam;

typedef struct {
	int StartUpPort = 8090;
	const char* ServerAddr = "127.0.0.1";
}CsClientParam;

typedef struct {
	int StartUpPort = 8080;
	const char* ServerAddr = "127.0.0.1";
}DsClientParam;


class ClientNet

{

private:

	SOCKET m_sock;

	SOCKET cs_sock;

	SOCKET ds_sock;

public:
	//分为发送计算结果的客户端和发送图像数据的客户端

	// 连接指定的服务器

	int ClientConnect(int port, const char* address);
	int CsClientConnect(int port, const char* address);
	int DsClientConnect(int port, const char* address);

	// 发送信息

	int ClientSend(const char* msg, int len);
	int CsClientSend(const char* msg, int len);
	int DsClientSend(const char* msg, int len);

	// 关闭连接

	void ClientClose();
	void CsClientClose();
	void DsClientClose();

};

