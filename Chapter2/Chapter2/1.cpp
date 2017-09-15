#pragma comment(lib, "ws2_32")

#include<WinSock2.h>
#include<iostream>

int main(int argc, char *argv[])
{
	// ��������� �ʱ�ȭ
	WSADATA wsa;
	WSADATA wsa1_1;

	// 2.2 ����
	// MAKEWORD(a, b)
	// a�� ���� b�� ����

	// 0�� ����, �����ϸ� 1
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
		return 1;
	}

	// ���� ��Ʈ�� �ֹ���
	// ���� ��Ʈ�� �ι���

	// Ws2_32���� ȣ���ڰ� ����Ұ����� �����ϴ� ������ ��� ����
	std::cout << "wVersion      : " << (wsa.wVersion & 0x000f)  << "." << ((wsa.wVersion >> 8 ) & 0x00ff ) << std::endl;

	// Ws2_32�� �����Ҽ� �ִ� �ִ� ���� ���
	std::cout << "wHighVersion  : " << (wsa.wHighVersion & 0x000f) << "." << ((wsa.wHighVersion >> 8) & 0x00ff ) << std::endl;

	// Ws2_32�� ���� ������ ���� ������ �����ϴ� NULL�� ������ ���ڿ�
	std::cout << "szDescription : " << wsa.szDescription << std::endl;

	// Ws2_32�� ���û��� �Ǵ� ���� ������ �����ϴ� NULL���� ������ ���ڿ�
	std::cout << "szSystemStatus: " << wsa.szSystemStatus << std::endl;

	std::cout << std::endl;

	// 1.1 ����
	if (WSAStartup(MAKEWORD(1, 1), &wsa1_1) != 0) {
		return 1;
	}

	std::cout << "wVersion      : " << (wsa1_1.wVersion & 0x000f) << "." << ((wsa1_1.wVersion >> 8 ) & 0x00ff ) << std::endl;
	std::cout << "wHighVersion  : " << (wsa1_1.wHighVersion & 0x00f) << "." << ((wsa1_1.wHighVersion >> 8) & 0x00ff ) << std::endl;
	std::cout << "szDescription : " << wsa1_1.szDescription << std::endl;
	std::cout << "szSystemStatus: " << wsa1_1.szSystemStatus << std::endl;

	MessageBox(NULL, "���� �ʱ�ȭ ����", "�˸�", MB_OK);

	// ������ ���� ����
	WSACleanup();

	return 0;
}