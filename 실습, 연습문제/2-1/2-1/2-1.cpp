#pragma comment(lib, "ws2_32")

#include<WinSock2.h>
#include<iostream>

int main(int argc, char *argv[]) 
{
	// ��������� �ʱ�ȭ
	WSADATA wsa;
	
	// 2.2 ����
	if (WSAStartup(MAKEWORD(2,2), &wsa) != 0) {
		return 1;
	}
	
	// ������ ���� ����
	WSACleanup();

	return 0;
}