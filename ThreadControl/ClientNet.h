#pragma once

#include<stdio.h>

#include<windows.h>



#pragma comment(lib, "Ws2_32.lib")

typedef struct {

	int CameraNum;
	int Framecnt;
	float Res[2048];

}SocketPackage;

class ClientNet

{

private:

	SOCKET m_sock;



public:

	// 连接指定的服务器

	int ClientConnect(int port, const char* address);

	// 发送信息

	int ClientSend(const char* msg, int len);

	// 关闭连接

	void ClientClose();

};

