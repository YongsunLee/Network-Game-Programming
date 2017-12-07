
#include "stdafx.h"

#define SERVERPORT 9000
#define BUFSIZE    512

CRITICAL_SECTION cs;
list<SOCKET> sockList;
list<QMsg>clientDatabuf;


SendMsg sendMsg;


CPlayer							m_Player[2];
Map								m_Map;

list<unique_ptr<CBlock>>		m_lstBlock;
list<unique_ptr<CBomb>>			m_lstBoom;

float GetLength(D2D_POINT_2F sz);

void OnCreate();
void MakeBoom(int ID);
void DecodeMsg(QMsg msg);
void Update(float fTimeElapsed);
CTimer m_timer;
UINT nClients = 0;
// 소켓 함수 오류 출력 후 종료
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
	QMsg test;


	while (1) {
		// 비어있지 않으면
		EnterCriticalSection(&cs);
		if (!clientDatabuf.empty()) {
			test = clientDatabuf.front();
			clientDatabuf.pop_front();
			DecodeMsg(test);
		}


		if (m_timer.Update()) {
		Update(m_timer.GetTimeElapsed());
			for (int i = 0; i < nClients; ++i) {
				sendMsg.playerPos[i] = m_Player[i].GetPosition();
				sendMsg.moveVec[i] = m_Player[i].GetDir();
				if (m_Player[i].GetActive()) sendMsg.status[i] = SendMsg::Living;
				else sendMsg.status[i] = SendMsg::Death;
			}
			sendMsg.nbombCnt = m_lstBoom.size();
			int bnum = 0;
			for (auto& p:m_lstBoom) {
				sendMsg.BombPos[bnum] = p->GetPosition();
				sendMsg.BombStat[bnum++] = p->GetBoom();
			}

			for (auto& p : sockList) {
				retval = send(p, (char *)&sendMsg, BUFSIZE, 0);
				if (retval == SOCKET_ERROR) {
					err_display("send()");
				}
			}
		}
		LeaveCriticalSection(&cs);

	}

	return 0;
}

// 클라이언트와 데이터 통신
DWORD WINAPI ProcessClient(LPVOID arg)
{
	SOCKET client_sock = (SOCKET)arg;
	int retval;
	SOCKADDR_IN clientaddr;
	int addrlen;
	char buf[BUFSIZE + 1];
	D2D_POINT_2F Pos{ 0,0 };
	UINT ID = nClients;
	// 클라이언트 정보 얻기
	addrlen = sizeof(clientaddr);
	getpeername(client_sock, (SOCKADDR *)&clientaddr, &addrlen);

	retval = send(client_sock, (char *)&nClients, BUFSIZE, 0);
	nClients++;

	if (retval == SOCKET_ERROR) {
		err_display("send()");
	}
	ClientMsg* recvMsg;
	while (1) {
		// 데이터 받기
		retval = recvn(client_sock, buf, BUFSIZE, 0);
		if (retval == SOCKET_ERROR) {
			err_display("recv()");
			break;
		}
		else if (retval == 0)
			break;

		buf[retval] = '\0';
		recvMsg = (ClientMsg*)buf;

		EnterCriticalSection(&cs);
		QMsg qmsg;
		qmsg.ID = ID;
		qmsg.msg = recvMsg;
		clientDatabuf.push_back(qmsg);

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

int main(int argc, char *argv[])
{
	int retval;

	InitializeCriticalSection(&cs);

	// 윈속 초기화
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

	// 데이터 통신에 사용할 변수
	SOCKET client_sock;
	SOCKADDR_IN clientaddr;
	int addrlen;
	HANDLE hThread;
	OnCreate();

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

		// 접속한 클라이언트 정보 출력
		printf("\n[TCP 서버] 클라이언트 접속: IP 주소=%s, 포트 번호=%d\n",
			inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));

		// 스레드 생성
		hThread = CreateThread(NULL, 0, ProcessClient,
			(LPVOID)client_sock, 0, NULL);
		if (hThread == NULL) { closesocket(client_sock); }
		else { CloseHandle(hThread); }

	}

	//
	DeleteCriticalSection(&cs);

	// closesocket()
	closesocket(listen_sock);

	// 윈속 종료
	WSACleanup();
	return 0;
}

void OnCreate()
{
	m_Player[0] = CPlayer(SizeU(0, 0));
	m_Player[1] = CPlayer(SizeU(11, 0));

	for (int i = 0; i < 12; ++i)
		for (int j = 0; j < 12; ++j)
		{
			if (m_Map.m_ppMap[i][j] == 1) {
				auto block = make_unique<CBlock>(SizeU(i, j));
				m_lstBlock.push_back(move(block));

			}
		}
}

void MakeBoom(int ID)
{
	printf("ID: %d, make_boom\n", ID);

	D2D1_SIZE_U retCoord = m_Player[ID].GetCoord();

	bool col = false;
	for (auto& p : m_lstBlock) {
		if (p->GetCoord() == retCoord) {
			col = true;
			break;
		}
	}
	if (!col) {

		auto item = make_unique<CBomb>(retCoord);

		m_lstBoom.push_back(move(item));
	}
}

void DecodeMsg(QMsg msg)
{
	int i = msg.ID;
	m_Player[i].SetMoveX(msg.msg->CheckData[0]);
	m_Player[i].SetMoveY(msg.msg->CheckData[1]);
	if (msg.msg->SetBomb)MakeBoom(i);
}

void Update(float fTimeElapsed)
{
	for (int i = 0; i < 2; ++i)
	{
		for (auto& p : m_lstBlock)
		{
			auto pos = m_Player[i].GetPosition() + m_Player[i].GetSize();
			if (p->Colided(pos + m_Player[i].GetMove()))
			{
				m_Player[i].Move(-1 * m_Player[i].GetMove(), fTimeElapsed);
			}
		}

		for (auto& p : m_lstBoom) {
			p->Update(fTimeElapsed);
			auto pos = m_Player[i].GetPosition() + m_Player[i].GetSize();
			if (p->GetBoom()) {
				if (p->ColideBoom(pos)) {
					m_Player[i].SetActive(false);
				}
				for (auto& p2 : m_lstBoom) {
					if (p != p2)
					{
						if (p->ColideBoom(p2->GetCoord())) {
							p2->SetBoom();
						}
					}
				}

			}
			if (p->Colided(pos + m_Player[i].GetMove()))
			{
				if (GetLength(m_Player[i].GetPosition() - p->GetPosition()) > m_Player[i].GetSize().bottom + p->GetSize().bottom)
					m_Player[i].Move(-1 * m_Player[i].GetMove(), fTimeElapsed);
			}
		}

		
	}
	for (auto iter = m_lstBoom.begin(); iter != m_lstBoom.end();) {
		if ((*iter)->GetBoom() && (*iter)->GetTime() <= 0) {
			iter = m_lstBoom.erase(iter);
		}
		else iter++;
	}
	for(int i=0 ; i<2 ;++i)
		m_Player[i].Update(fTimeElapsed);

}

float GetLength(D2D_POINT_2F sz) { return sqrt((sz.x*sz.x) + (sz.y*sz.y)); }
