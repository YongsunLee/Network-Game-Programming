
#include "stdafx.h"

#define SERVERPORT 9000
#define BUFSIZE    512

CRITICAL_SECTION cs;
list<SOCKET> sockList;
list<ClientMsg*>clientDatabuf;


ClientMsg clientData[2];
TestPlayer player[2];


CTimer m_timer;
UINT nClients=0;
// ���� �Լ� ���� ��� �� ����
void err_quit(char *msg)
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

// ���� �Լ� ���� ���
void err_display(char *msg)
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

int recvn(SOCKET s, char *buf, int len, int flags)
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

DWORD WINAPI test(LPVOID arg) {
	int retval;
	ClientMsg test;


	while (1) {
		EnterCriticalSection(&cs);
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
		if (clientDatabuf.begin() != clientDatabuf.end()) {
			test = *clientDatabuf.front();
			clientData[test.ID].CheckData[0] = test.CheckData[0];
			clientData[test.ID].CheckData[1] = test.CheckData[1];
			clientDatabuf.pop_front();
		}

		LeaveCriticalSection(&cs);
	}

	return 0;
}

// Ŭ���̾�Ʈ�� ������ ���
DWORD WINAPI ProcessClient(LPVOID arg)
{
	SOCKET client_sock = (SOCKET)arg;
	int retval;
	SOCKADDR_IN clientaddr;
	int addrlen;
	char buf[BUFSIZE + 1];
	D2D_POINT_2F Pos{ 0,0 };

	// Ŭ���̾�Ʈ ���� ���
	addrlen = sizeof(clientaddr);
	getpeername(client_sock, (SOCKADDR *)&clientaddr, &addrlen);

	retval = send(client_sock, (char *)&nClients, BUFSIZE, 0);
	nClients++;

	if (retval == SOCKET_ERROR) {
		err_display("send()");
	}
	ClientMsg* recvMsg;
	while (1) {
		// ������ �ޱ�
		retval = recvn(client_sock, buf, BUFSIZE, 0);
		if (retval == SOCKET_ERROR) {
			err_display("recv()");
			break;
		}
		else if (retval == 0)
			break;

		EnterCriticalSection(&cs);
		buf[retval] = '\0';
		recvMsg = (ClientMsg*)buf;
		// ���� ������ ���
		printf("[TCP/%s:%d] %f,  %f\n", inet_ntoa(clientaddr.sin_addr),
			ntohs(clientaddr.sin_port), recvMsg->CheckData[0], recvMsg->CheckData[1]);
		clientDatabuf.push_back(recvMsg);

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
	printf("[TCP ����] Ŭ���̾�Ʈ ����: IP �ּ�=%s, ��Ʈ ��ȣ=%d\n",
		inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));

	return 0;
}

int main(int argc, char *argv[])
{
	int retval;

	InitializeCriticalSection(&cs);

	// ���� �ʱ�ȭ
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;

	//////////////////////////
	//////////////////////////

	// socket()
	SOCKET listen_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (listen_sock == INVALID_SOCKET) err_quit("socket()");

	// bind()
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

	// ������ ��ſ� ����� ����
	SOCKET client_sock;
	SOCKADDR_IN clientaddr;
	int addrlen;
	HANDLE hThread;

	hThread = CreateThread(NULL, 0, test, 0, 0, NULL);
	if (hThread == NULL) { closesocket(client_sock); }
	else { CloseHandle(hThread); }

	while (1) {
		// accept()
		addrlen = sizeof(clientaddr);
		client_sock = accept(listen_sock, (SOCKADDR *)&clientaddr, &addrlen);
		if (client_sock == INVALID_SOCKET) {
			err_display("accept()");
			break;
		}

		EnterCriticalSection(&cs);
		sockList.push_back(client_sock);
		LeaveCriticalSection(&cs);

		// ������ Ŭ���̾�Ʈ ���� ���
		printf("\n[TCP ����] Ŭ���̾�Ʈ ����: IP �ּ�=%s, ��Ʈ ��ȣ=%d\n",
			inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));

		// ������ ����
		hThread = CreateThread(NULL, 0, ProcessClient,
			(LPVOID)client_sock, 0, NULL);
		if (hThread == NULL) { closesocket(client_sock); }
		else { CloseHandle(hThread); }

	}

	//
	DeleteCriticalSection(&cs);

	// closesocket()
	closesocket(listen_sock);

	// ���� ����
	WSACleanup();
	return 0;
}