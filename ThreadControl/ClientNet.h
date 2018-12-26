#pragma once

#include<stdio.h>

#include<windows.h>



#pragma comment(lib, "Ws2_32.lib")

typedef struct {

	int CameraNum;
	int Framecnt;
	float Res[2048];

}SocketPackage;

typedef struct {
	int StartUpPort = 8001;
	const char* ServerAddr = "127.0.0.1";
}ClientParam;

class ClientNet

{

private:

	SOCKET m_sock;



public:

	// ����ָ���ķ�����

	int ClientConnect(int port, const char* address);

	// ������Ϣ

	int ClientSend(const char* msg, int len);

	// �ر�����

	void ClientClose();

};
