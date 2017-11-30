#pragma comment(lib, "ws2_32")
#include <winsock2.h>
#include <stdlib.h>
#include <stdio.h>

#define SERVERIP   "127.0.0.1"
#define SERVERPORT 9000
#define BUFSIZE    512

struct Vec2 {
	int x;
	int y;
};

// �׽�Ʈ�� ����ü
struct ClientMsg {
	UINT ID;
	float CheckData[2];
	//D2D_POINT_2F Dir;
};

struct TestPlayer {
	float pos[2];
};

TestPlayer player[2];
UINT ClientID;
// ���� �Լ� ���� ��� �� ����
void err_quit(char *msg)
{
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER|FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);
	MessageBox(NULL, (LPCTSTR)lpMsgBuf, msg, MB_ICONERROR);
	LocalFree(lpMsgBuf);
	exit(1);
}

// ���� �Լ� ���� ���
void err_display(char *msg)
{
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER|FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);
	printf("[%s] %s", msg, (char *)lpMsgBuf);
	LocalFree(lpMsgBuf);
}

// ����� ���� ������ ���� �Լ�
int recvn(SOCKET s, char *buf, int len, int flags)
{
	int received;
	char *ptr = buf;
	int left = len;

	while(left > 0){
		received = recv(s, ptr, left, flags);
		if(received == SOCKET_ERROR)
			return SOCKET_ERROR;
		else if(received == 0)
			break;
		left -= received;
		ptr += received;
	}

	return (len - left);
}

DWORD WINAPI test(LPVOID arg) {

	SOCKET sock = (SOCKET)arg;
	int retval;
	char buf[BUFSIZE];
	ClientMsg testMsg;



	while (1) {
		// ������ �ޱ�
		retval = recvn(sock, buf, BUFSIZE, 0);
		if (retval == SOCKET_ERROR) {
			err_display("recv()");
			break;
		}
		else if (retval == 0)
			break;

		memcpy(&player, buf, retval);

		// ���� ������ ���
		for (int i = 0; i < 2; ++i) {

		printf("%f,  %f\n", player[i].pos[0], player[i].pos[1]);
		}
	}
	return 0;
}

void ClientSend(char* buf, SOCKET sock) {
	int retval;

	// ������ ������
	retval = send(sock, buf, BUFSIZE, 0);
	if (retval == SOCKET_ERROR) {
		err_display("send()");
	}
};

int main(int argc, char *argv[])
{
	int retval;
	HANDLE hThread;

	ClientMsg testmsg;

	// ���� �ʱ�ȭ
	WSADATA wsa;
	if(WSAStartup(MAKEWORD(2,2), &wsa) != 0)
		return 1;

	// socket()
	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	if(sock == INVALID_SOCKET) err_quit("socket()");

	// connect()
	SOCKADDR_IN serveraddr;
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = inet_addr(SERVERIP);
	serveraddr.sin_port = htons(SERVERPORT);
	retval = connect(sock, (SOCKADDR *)&serveraddr, sizeof(serveraddr));
	if(retval == SOCKET_ERROR) err_quit("connect()");

	char buf[BUFSIZE];


	retval = recvn(sock, buf, BUFSIZE, 0);
	if (retval == SOCKET_ERROR) {
		err_display("recv()");
		return 0;
	}
	memcpy(&ClientID, buf, retval);
	hThread = CreateThread(NULL, 0, test,
		(LPVOID)sock, 0, NULL);
	if (hThread == NULL) { closesocket(sock); }
	else { CloseHandle(hThread); }

	// ������ ��ſ� ����� ����
	int len;

	testmsg.ID = ClientID;
	testmsg.CheckData[0] = 1;
	testmsg.CheckData[1] = 0;

	// ������ ������ ���
	while(1){
		ClientSend((char *)&testmsg, sock);

	}

	// closesocket()
	closesocket(sock);

	// ���� ����
	WSACleanup();
	return 0;
}