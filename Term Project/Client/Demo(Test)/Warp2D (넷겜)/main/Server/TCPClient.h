#pragma once

#include "Object\Player\Player.h"

#define SERVERIP   "127.0.0.1"
#define SERVERPORT 9000
#define BUFSIZE    512

class CClient{
public:
	CClient();
	~CClient();

	int recvn(SOCKET s, char *buf, int len, int flags);

	void err_display(char *msg);
	void err_quit(char *msg);
	void ClientSend(ClientMsg msg);
	//ClientMsg MakeMsg();
	ClientMsg ClientRecive();

private:
	SOCKET sock;
	WSADATA wsa;
	ClientMsg m_recvMsg;
};