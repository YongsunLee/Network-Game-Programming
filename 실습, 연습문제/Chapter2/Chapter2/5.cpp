#pragma comment(lib, "ws2_32")

#include<WinSock2.h>

void err_quit(char* msg);
int f(int x);

int main(int argc, char *argv[])
{
	// ��������� �ʱ�ȭ
	WSADATA wsa;


	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
		return 1;
	}
	MessageBox(NULL, "���� �ʱ�ȭ ����", "�˸�", MB_OK);

	// socket()
	SOCKET tcp_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (tcp_sock == INVALID_SOCKET) err_quit("socket()");
	MessageBox(NULL, "TCP ���� ����", "�˸�", MB_OK);

	int retval = f(10);
	if (retval == SOCKET_ERROR) err_quit("f()");

	// closesocket()
	closesocket(tcp_sock);

	// ������ ���� ����
	WSACleanup();
	return 0;
}

// ���� �Լ� ���� ��� �� ����
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

// x�� 0 �Ǵ� ����� ����
// ���� ���� 0�̰�, �����ڵ�� 0���� ����
// x�� ������ ����
// ���� ���� SOKET_ERROR�̰�, �����ڵ�� WSAEMSGSIZE�� ����
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