#include "stdafx.h"

#define SERVERIP "127.0.0.1"
#define SERVERPORT 9000
#define BUFSIZE 512

// 소켓 함수 오류 출력 후 종료
void err_quit(char *msg)
{
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
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
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL
	);

	printf("[%s] %s", msg, (char *)lpMsgBuf);

	LocalFree(lpMsgBuf);
}

// 사용자 정의 데이터 수신 함수
int recvn(SOCKET s, char *buf, int len, int flags) {
	int received;
	char *ptr = buf;
	int left = len;

	while (left > 0) {
		received = recv(s, ptr, left, flags);
		if (received == SOCKET_ERROR) return SOCKET_ERROR;
		else if (received == 0) break;
		left -= received;	// 남은 길이 받은만큼 빼고
		ptr += received;	// 다음값 주소 받은만큼 더하고
	}

	return (len - left);
}

int main(int argc, char *argv[])
{
	int retval;
	char inputIP[256];
	int maxPort = 0;
	int minPort = 0;
	int loopNum = 0;
	int check =0;

	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) return 1;

	// socket()
	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET) err_quit("socket()");

	// IP, 포트번호 입력
	printf("IP를 입력해주세요 :");
	scanf("%s", &inputIP);
	
	printf("\n포트번호를 입력해주세요(최소) :");
	scanf("%d", &minPort);

	printf("\n포트번호를 입력해주세요(최대) :");
	scanf("%d", &maxPort);


	loopNum = minPort;

	// connect()
	SOCKADDR_IN serveraddr;
	::ZeroMemory(&serveraddr, sizeof(SOCKADDR_IN));

	while (loopNum != maxPort) {
		serveraddr.sin_family = AF_INET;
		serveraddr.sin_addr.S_un.S_addr = inet_addr(inputIP);
		serveraddr.sin_port = htons(loopNum);
		retval = connect(sock, (SOCKADDR *)&serveraddr, sizeof(serveraddr));
		if (retval == SOCKET_ERROR) printf("%d", ++check);
		else printf("\n%d", loopNum);
		if (loopNum == maxPort)	break;
		++loopNum;
	}

	/*
	// 데이터 통신에 사용할 변수
	int len;
	char buf[BUFSIZE + 1];

	while (1) {
		// 데이터 입력
		printf("\n[보낼 데이터] ");
		if (fgets(buf, BUFSIZE + 1, stdin) == NULL) break;

		// '\n' 문자 제거
		len = strlen(buf);
		if (buf[len - 1] == '\n') buf[len - 1] = '\0';
		if (strlen(buf) == 0) break;

		// 데이터 보내기
		retval = send(sock, buf, strlen(buf), 0);
		if (retval == SOCKET_ERROR) {
			err_quit("send()");
			break;
		}

		printf("[TCP 클라이언트] %d바이트를 보냈습니다.\n", retval);

		// 데이터 받기
		retval = recvn(sock, buf, retval, 0);
		if (retval == SOCKET_ERROR) {
			err_display("recv()");
			break;
		}
		else if (retval == 0) break;

		// 받은 데이터 출력
		buf[retval] = '\0';
		printf("[TCP 클라이언트] %d바이트를 받았습니다.\n", retval);
		printf("[받은 데이터] %s\n", buf);
	}
	*/

	// closesocket()
	closesocket(sock);

	WSACleanup();
	return 0;
}
