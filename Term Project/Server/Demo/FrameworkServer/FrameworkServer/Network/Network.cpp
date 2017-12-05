#include "stdafx.h"
#include "Framework\Timer\Timer.h"
#include "Network.h"

CNetwork::CNetwork()
{
	int retval;

	InitializeCriticalSection(&cs);

	// 윈속 초기화
	WSADATA wsa;
	WSAStartup(MAKEWORD(2, 2), &wsa);

	// socket()
	listen_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (listen_sock == INVALID_SOCKET) err_quit("socket()");

	SOCKADDR_IN serveraddr;
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serveraddr.sin_port = htons(SERVERPORT);
	retval = bind(listen_sock, (SOCKADDR *)&serveraddr, sizeof(serveraddr));
	if (retval == SOCKET_ERROR) err_quit("bind()");

	// listen()
	retval = listen(listen_sock, SOMAXCONN);
	if (retval == SOCKET_ERROR) err_quit("listen()");


}

CNetwork::~CNetwork()
{
	DeleteCriticalSection(&cs);

	// closesocket()
	closesocket(listen_sock);

	// 윈속 종료
	WSACleanup();
}

void CNetwork::err_quit(char * msg)
{
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);
	MessageBox(NULL, (LPCTSTR)lpMsgBuf, (LPCTSTR)msg, MB_ICONERROR);
	LocalFree(lpMsgBuf);
	exit(1);
}

void CNetwork::err_display(char * msg)
{
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);
	printf("[%s] %s", msg, (char *)lpMsgBuf);
	LocalFree(lpMsgBuf);
}

int CNetwork::recvn(SOCKET s, char * buf, int len, int flags)
{
	int received;
	char *ptr = buf;
	int left = len;

	while (left > 0) {
		received = recv(s, ptr, left, flags);
		if (received == SOCKET_ERROR)
			return SOCKET_ERROR;
		else if (received == 0)
			break;
		left -= received;
		ptr += received;
	}

	return (len - left);
}

DWORD ServerRecv(LPVOID arg) {
	SOCKET client_sock = (SOCKET)arg;
	SOCKADDR_IN clientaddr;
	int retval;
	int addrlen;
	char buf[BUFSIZE + 1];

	// 클라이언트 정보 얻기
	addrlen = sizeof(clientaddr);
	getpeername(client_sock, (SOCKADDR *)&clientaddr, &addrlen);

	while (1) {
		// 데이터 받기
		retval = recvn(client_sock, buf, BUFSIZE, 0);
		if (retval == SOCKET_ERROR) {
			err_display("recv()");
			break;
		}
		else if (retval == 0)
			break;

		EnterCriticalSection(&cs);

		LeaveCriticalSection(&cs);
	}

	EnterCriticalSection(&cs);

	for (auto iter = sockList.begin(); iter != sockList.end(); ++iter)
	{
		if (*iter == client_sock) {
			sockList.erase(iter);
			break;
		}
	}

	LeaveCriticalSection(&cs);

	// closesocket()
	closesocket(client_sock);

	printf("[TCP 서버] 클라이언트 종료: IP 주소=%s, 포트 번호=%d\n",
		inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));

	return 0;
}

DWORD ServerSend(LPVOID arg)
{
	int retval;

	while (1) {
		EnterCriticalSection(&cs);
		// 비어있지 않으면
		if (clientDatabuf.begin() != clientDatabuf.end()) {
			test = *clientDatabuf.front();
			clientData[test.ID].CheckData[0] = test.CheckData[0];	// x
			clientData[test.ID].CheckData[1] = test.CheckData[1];	// y
			clientDatabuf.pop_front();
		}

		if (m_timer.Update()) {
			for (int i = 0; i < nClients; ++i) {
				player[i].pos[0] += m_timer.GetTimeElapsed()*clientData[i].CheckData[0];
				player[i].pos[1] += m_timer.GetTimeElapsed()*clientData[i].CheckData[1];
			}

			for (auto& p : sockList) {
				retval = send(p, (char *)&player, BUFSIZE, 0);
				if (retval == SOCKET_ERROR) {
					err_display("send()");
				}
			}
		}

		LeaveCriticalSection(&cs);
	}

	return 0;
}

