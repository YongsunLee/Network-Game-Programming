#pragma comment(lib, "ws2_32")

#include<WinSock2.h>

void err_quit(char* msg);
int f(int x);

int main(int argc, char *argv[])
{
	// 윈도우소켓 초기화
	WSADATA wsa;


	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
		return 1;
	}
	MessageBox(NULL, "윈속 초기화 성공", "알림", MB_OK);

	// socket()
	SOCKET tcp_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (tcp_sock == INVALID_SOCKET) err_quit("socket()");
	MessageBox(NULL, "TCP 소켓 성공", "알림", MB_OK);

	int retval = f(10);
	if (retval == SOCKET_ERROR) err_quit("f()");

	// closesocket()
	closesocket(tcp_sock);

	// 윈도우 소켓 종료
	WSACleanup();
	return 0;
}

// 소켓 함수 오류 출력 후 종료
void err_quit(char* msg)
{
	LPVOID lpMsgBuf;
	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL,
		WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf,
		0,
		NULL);
	MessageBox(NULL, (LPCTSTR)lpMsgBuf, msg, MB_ICONERROR);
	LocalFree(lpMsgBuf);
	exit(1);
}

// x가 0 또는 양수면 성공
// 리턴 값은 0이고, 오류코드는 0으로 설정
// x가 음수면 실패
// 리턴 값은 SOKET_ERROR이고, 오류코드는 WSAEMSGSIZE로 설정
int f(int x)
{
	if (x == 0 || x > 0)
	{
		WSASetLastError(0);
		return 0;
	}
	else if (x < 0) 
	{
		WSASetLastError(WSAEMSGSIZE);
		return SOCKET_ERROR;
	}
}