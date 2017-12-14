#include "stdafx.h"

#define SERVERPORT 9000
#define BUFSIZE    1024

CRITICAL_SECTION				cs;
list<SoketMgr>					sockList;
list<QueueMsg>					clientDatabuf;

ServerSendMsg					sendMsg;

CPlayer							*m_Player[2];
Map								m_Map;

list<unique_ptr<CBlock>>		m_lstBlock;
list<unique_ptr<CBomb>>			m_lstBoom;

CTimer							m_timer;
float							m_fResetTimer{3};
bool							m_bGameOver{ false };
void OnCreate();
void MakeBoom(int ID);
void DecodeMsg(QueueMsg msg);
void Update(float fTimeElapsed);
void Reset();
float GetLength(D2D_POINT_2F sz);
int GetID();
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

DWORD WINAPI ServerSend(LPVOID arg) {
	int retval;
	QueueMsg temp;

	while (1) {
		EnterCriticalSection(&cs);

		// 비어있지 않으면
		if (!clientDatabuf.empty()) {

			temp = clientDatabuf.front();
			clientDatabuf.pop_front();

			DecodeMsg(temp);
		}

		if (m_timer.Update()) {

			Update(m_timer.GetTimeElapsed());
			for (auto& p : sockList) {
				int i = p.ID;
				// 위치, 방향
				sendMsg.playerPos[i] = m_Player[i]->GetPosition();
				sendMsg.moveVec[i] = m_Player[i]->GetDir();

				// 죽었는지 살았는지
				if (m_Player[i]->GetActive()) sendMsg.status[i] = ServerSendMsg::Living;
				else sendMsg.status[i] = ServerSendMsg::Death;
			}

			sendMsg.nbombCnt = m_lstBoom.size();
			int bnum = 0;

			for (auto& p : m_lstBoom) {
				sendMsg.BombPos[bnum] = p->GetPosition();
				if (p->GetBoom()) sendMsg.BombStat[bnum] = ServerSendMsg::BOOM;
				else sendMsg.BombStat[bnum] = ServerSendMsg::SET;
				bnum++;
			}

			for (auto& p : sockList) {
				retval = send(p.sock, (char *)&sendMsg, BUFSIZE, 0);
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
DWORD WINAPI ServerRecv(void* arg)
{
	SoketMgr* sockMgr = (SoketMgr*)arg;
	SOCKET client_sock = sockMgr->sock;
	SOCKADDR_IN clientaddr;
	UINT ID = sockMgr->ID;
	int retval;
	int addrlen;
	char buf[BUFSIZE + 1];

	// 클라이언트 정보 얻기
	addrlen = sizeof(clientaddr);
	getpeername(client_sock, (SOCKADDR *)&clientaddr, &addrlen);

	// 클라이언트의 ID전송
	// 초기 시작위치 ID에 해당하는
	// ID 값 제한

	retval = send(client_sock, (char *)&ID, BUFSIZE, 0);

	ClientSendMsg* recvMsg;
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
		recvMsg = (ClientSendMsg*)buf;

		EnterCriticalSection(&cs);
		QueueMsg Qmsg;
		Qmsg.ID = ID;
		Qmsg.msg = recvMsg;
		clientDatabuf.push_back(Qmsg);
		LeaveCriticalSection(&cs);
	}

	EnterCriticalSection(&cs);
	for (auto iter = sockList.begin(); iter != sockList.end(); ++iter)
	{
		if ((*iter).sock == client_sock) {
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
	HANDLE hThread;
	int addrlen;

	// 초기화 (플레이어 위치, 맵)
	OnCreate();

	hThread = CreateThread(NULL, 0, ServerSend, 0, 0, NULL);
	if (hThread != NULL) { CloseHandle(hThread); }

	while (1) {
		// accept()
		addrlen = sizeof(clientaddr);
		client_sock = accept(listen_sock, (SOCKADDR *)&clientaddr, &addrlen);
		if (client_sock == INVALID_SOCKET) {
			err_display("accept()");
			break;
		}


		EnterCriticalSection(&cs);
		// SocketList Client push
		SoketMgr tmp;
		int val = GetID();
		if (val == -1) {
			closesocket(client_sock);
		}
		else
		{
			tmp.sock = client_sock;
			tmp.ID = val;
			sockList.push_back(tmp);
		}


		// 접속한 클라이언트 정보 출력
		printf("\n[TCP 서버] 클라이언트 접속: IP 주소=%s, 포트 번호=%d\n",
			inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));

		// 스레드 생성
		hThread = CreateThread(NULL, 0, ServerRecv, &tmp, 0, NULL);
		if (hThread == NULL) { closesocket(client_sock); }
		else { CloseHandle(hThread); }
		LeaveCriticalSection(&cs);
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
	m_bGameOver = false;
	m_Player[0] = new CPlayer(SizeU(0, 0));
	m_Player[1] = new CPlayer(SizeU(11, 0));

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

	D2D1_SIZE_U retCoord = m_Player[ID]->GetCoord();

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

void DecodeMsg(QueueMsg msg)
{
	int i = msg.ID;
	m_Player[i]->SetMoveX(msg.msg->CheckData[0]);
	m_Player[i]->SetMoveY(msg.msg->CheckData[1]);
	if (msg.msg->SetBomb)MakeBoom(i);
}

void Update(float fTimeElapsed)
{
	for (int i = 0; i < 2; ++i)
	{
		for (auto& p : m_lstBlock)
		{
			auto pos = m_Player[i]->GetPosition() + m_Player[i]->GetSize();
			if (p->Colided(pos + m_Player[i]->GetMove()))
			{
				m_Player[i]->Move(-1 * m_Player[i]->GetMove(), fTimeElapsed);
			}
		}

		for (auto& p : m_lstBoom) {
			p->Update(fTimeElapsed);
			auto pos = m_Player[i]->GetPosition() + m_Player[i]->GetSize();
			if (p->GetBoom()) {
				if (p->ColideBoom(pos)) {
					m_Player[i]->SetActive(false);
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
			if (p->Colided(pos + m_Player[i]->GetMove()))
			{
				if (GetLength(m_Player[i]->GetPosition() - p->GetPosition()) > m_Player[i]->GetSize().bottom + p->GetSize().bottom)
					m_Player[i]->Move(-1 * m_Player[i]->GetMove(), fTimeElapsed);
			}
		}
	}
	for (auto iter = m_lstBoom.begin(); iter != m_lstBoom.end();) {
		if ((*iter)->GetBoom() && (*iter)->GetTime() <= 0) {
			iter = m_lstBoom.erase(iter);
		}
		else iter++;
	}
	if (!m_bGameOver)
	{
		for (int i = 0; i < 2; ++i)
			m_Player[i]->Update(fTimeElapsed);
		if (!m_Player[0]->GetActive() || !m_Player[1]->GetActive()) {
			m_bGameOver = true;
		}
	}
	else {
		m_fResetTimer -= fTimeElapsed;
		if (m_fResetTimer <= 0) {
			Reset();
		}
	}
	
}

void Reset()
{
	m_fResetTimer = 3;
	m_bGameOver = false;
	m_Player[0]->SetPosByCoord(SizeU(0, 0));
	m_Player[0]->SetActive(true);

	m_Player[1]->SetPosByCoord(SizeU(11, 0));
	m_Player[1]->SetActive(true);

}

float GetLength(D2D_POINT_2F sz) { return sqrt((sz.x*sz.x) + (sz.y*sz.y)); }

int GetID()
{
	bool retval[2] = { false,false };
	for (auto& p : sockList) {
		retval[p.ID] = true;
	}
	if (!retval[0]) return 0;
	else if (!retval[1]) return 1;
	return -1;
}
