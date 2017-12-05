#pragma once
#include "stdafx.h"

class CNetwork {
private:
public:
	CRITICAL_SECTION cs;
	HANDLE hThread;

	SOCKET sock;
	TestPlayer player[2];
	UINT ClientID;

	D2D_POINT_2F m_retMsg;

	CNetwork();
	~CNetwork();

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

	// 소켓 함수 오류 출력
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

	// 사용자 정의 데이터 수신 함수
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
	int OnCreate();

	void ClientSend(char* buf, SOCKET sock);

	void MakeMsg(D2D_POINT_2F dir);

	D2D_POINT_2F GetMsg() { 
		EnterCriticalSection(&cs);
		m_retMsg.x = player[0].pos[0];
		m_retMsg.y = player[0].pos[1];
		LeaveCriticalSection(&cs);
		
		return m_retMsg; 
	}
};
static DWORD WINAPI test(LPVOID arg);