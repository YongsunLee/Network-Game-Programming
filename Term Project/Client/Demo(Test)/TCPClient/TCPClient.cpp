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

// 테스트용 구조체
struct ClientMsg {
	int CheckData;
	//Vec2 Pos;
};


// 소켓 함수 오류 출력 후 종료
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

// 소켓 함수 오류 출력
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

// 사용자 정의 데이터 수신 함수
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
		// 데이터 받기
		retval = recvn(sock, buf, BUFSIZE, 0);
		if (retval == SOCKET_ERROR) {
			err_display("recv()");
			break;
		}
		else if (retval == 0)
			break;

		memcpy(&testMsg, buf, retval);

		// 받은 데이터 출력
		printf("%d\n", testMsg.CheckData);
	}
	return 0;
}

void ClientSend(char* buf, SOCKET sock) {
	int retval;

	// 데이터 보내기
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

	// 윈속 초기화
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

	hThread = CreateThread(NULL, 0, test,
		(LPVOID)sock, 0, NULL);
	if (hThread == NULL) { closesocket(sock); }
	else { CloseHandle(hThread); }

	// 데이터 통신에 사용할 변수
	char buf[BUFSIZE+1];
	int len;

	testmsg.CheckData = 1;

	// 서버와 데이터 통신
	while(1){
		ClientSend((char *)&testmsg, sock);

	}

	// closesocket()
	closesocket(sock);

	// 윈속 종료
	WSACleanup();
	return 0;
}