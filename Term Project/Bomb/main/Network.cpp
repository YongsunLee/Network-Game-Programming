#include "stdafx.h"
#include "Network.h"

CNetwork::CNetwork()
{

	ClientMsg testmsg;
	InitializeCriticalSection(&cs);

	// 윈속 초기화
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return;

	// socket()
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET) err_quit("socket()");

	
	int retval;

	// connect()
	SOCKADDR_IN serveraddr;
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = inet_addr(SERVERIP);
	serveraddr.sin_port = htons(SERVERPORT);
	retval = connect(sock, (SOCKADDR *)&serveraddr, sizeof(serveraddr));
	if (retval == SOCKET_ERROR) err_quit("connect()");
	char buf[BUFSIZE];


	retval = recvn(sock, buf, BUFSIZE, 0);
	if (retval == SOCKET_ERROR) {
		err_display("recv()");
		return ;
	}
	memcpy(&ClientID, buf, retval);


	hThread = CreateThread(NULL, 0, test,
		(LPVOID)this, 0, NULL);
	if (hThread == NULL) { closesocket(sock); }
	else { CloseHandle(hThread); }
}

CNetwork::~CNetwork()
{
	// closesocket()
	TerminateThread(hThread,0);
	closesocket(sock);

	DeleteCriticalSection(&cs);

	// 윈속 종료
	WSACleanup();
}

int CNetwork::OnCreate()
{
	
	return 0;
}


void CNetwork::ClientSend(char * buf, SOCKET sock)
{
	int retval;

	// 데이터 보내기
	retval = send(sock, buf, BUFSIZE, 0);
	if (retval == SOCKET_ERROR) {
		err_display("send()");
	}
}

void CNetwork::MakeMsg(D2D_POINT_2F dir, bool Bomb)
{
	ClientMsg testmsg;

	testmsg.CheckData[0] = dir.x;
	testmsg.CheckData[1] = dir.y;
	testmsg.SetBomb = Bomb;
	ClientSend((char *)&testmsg, sock);

}

DWORD  WINAPI test(LPVOID arg)
{
	CNetwork* network = (CNetwork*)arg;
	int retval;
	char buf[BUFSIZE];
	ClientMsg testMsg;



	while (1) {
		// 데이터 받기
		retval = network->recvn(network->sock, buf, BUFSIZE, 0);
		if (retval == SOCKET_ERROR) {
			network->err_display("recv()");
			break;
		}
		else if (retval == 0)
			break;
		EnterCriticalSection(&network->cs);
		memcpy(&network->recvMsg, buf, retval);

		LeaveCriticalSection(&network->cs);
		
	}
	return 0;
}
