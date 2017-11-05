#pragma comment(lib, "ws2_32")

#include<WinSock2.h>
#include<iostream>

int main(int argc, char *argv[]) 
{
	// 윈도우소켓 초기화
	WSADATA wsa;
	
	// 2.2 버전
	if (WSAStartup(MAKEWORD(2,2), &wsa) != 0) {
		return 1;
	}
	
	// 윈도우 소켓 종료
	WSACleanup();

	return 0;
}