#pragma once
#include "stdafx.h"
#include "Object\Player\Player.h"

struct SendMsg {
	enum PlayerStatus { Living, Death };
	D2D_POINT_2F playerPos[2];
	CPlayer::Dir moveVec[2];
	PlayerStatus status[2];
	int nbombCnt;
	D2D_POINT_2F BombPos[144];
	bool BombStat[144];
};

class CNetwork {
private:
public:
	CRITICAL_SECTION cs;
	HANDLE hThread;

	SOCKET sock;
	SendMsg recvMsg;
	UINT ClientID;


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

	void MakeMsg(D2D_POINT_2F dir,  bool makeBomb);

	D2D_POINT_2F GetPosition(int num) { 
		D2D_POINT_2F m_retMsg;
		EnterCriticalSection(&cs);
		m_retMsg = recvMsg.playerPos[num];
		LeaveCriticalSection(&cs);
		
		return m_retMsg; 
	}
	CPlayer::Dir GetMove(int num) {
		CPlayer::Dir m_retMsg;
		EnterCriticalSection(&cs);
		m_retMsg = recvMsg.moveVec[num];
		LeaveCriticalSection(&cs);

		return m_retMsg;
	}
	bool GetActive(int num) {
		bool retval;
		EnterCriticalSection(&cs);
		if (recvMsg.status[num] == SendMsg::Living) retval = true;
		else retval= false;
		LeaveCriticalSection(&cs);
		return retval;
	}
	int GetBombCnt() {
		int retval;
		EnterCriticalSection(&cs);
		retval = recvMsg.nbombCnt;
		LeaveCriticalSection(&cs);
		return retval;
	}
	D2D_POINT_2F GetBombPos(int num) {
		D2D_POINT_2F retval;
		EnterCriticalSection(&cs);
		retval = recvMsg.BombPos[num];
		LeaveCriticalSection(&cs);
		return retval;
	}
	bool GetBombState(int num) {
		bool retval;
		EnterCriticalSection(&cs);
		retval = recvMsg.BombStat[num];
		LeaveCriticalSection(&cs);
		return retval;
	}

};
static DWORD WINAPI test(LPVOID arg);