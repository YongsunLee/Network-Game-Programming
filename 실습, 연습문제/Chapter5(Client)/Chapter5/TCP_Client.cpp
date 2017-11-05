#pragma comment(lib, "ws2_32")
#include <winsock2.h>
#include <stdlib.h>
#include <stdio.h>

//#define SERVERIP   "192.168.0.13"
#define SERVERIP   "127.0.0.1"
#define SERVERPORT 9000
#define BUFSIZE 512

// ���� �Լ� ���� ��� �� ����
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

// ���� �Լ� ���� ���
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

int main(int argc, char *argv[])
{
	int retval;
	FILE *fp;
	
	// ���� �ʱ�ȭ
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;

	// socket()
	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET) err_quit("socket()");

	// connect()
	SOCKADDR_IN serveraddr;
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = inet_addr(SERVERIP);
	serveraddr.sin_port = htons(SERVERPORT);
	retval = connect(sock, (SOCKADDR *)&serveraddr, sizeof(serveraddr));
	if (retval == SOCKET_ERROR) err_quit("connect()");

	// ������ ��ſ� ����� ����

	char *buf;
	int len = 0;

	//if ((fp = fopen("./1.jpg", "rb")) == NULL) return -1;
	if ((fp = fopen("./poppin.zip", "rb")) == NULL) return -1;

	fseek(fp, 0, SEEK_END);
	len = ftell(fp);
	fseek(fp, 0, SEEK_SET);	// �� ó���� ������

	// �����Ҵ�
	buf = (char*)malloc(sizeof(char) * len);

	// ������ ������ ���
	// ���� ���� (���� ũ��)
	retval = send(sock, (char *)&len, sizeof(int), 0);
	if (retval == SOCKET_ERROR) {
		err_display("send()");
	}

	fread(buf, sizeof(char), len, fp);

	// ������ ������(���� ����)
	retval = send(sock, buf, len, 0);
	if (retval == SOCKET_ERROR) {
		err_display("send()");
	}
	printf("[TCP Ŭ���̾�Ʈ] %d+%d����Ʈ�� "
		"���½��ϴ�.\n", sizeof(void*), retval);
	// free
	free(buf);

	// ���� ������ ���
	/*

	char buf[BUFSIZE + 1];
	int len;

	fp = fopen("./2.mp4", "rb");
	
	fseek(fp, 0, SEEK_END);
	len = ftell(fp);
	fseek(fp, 0, SEEK_SET);	// �� ó���� ������

	retval = send(sock, (char *)&len, sizeof(int), 0);
	if (retval == SOCKET_ERROR) {
		err_display("send()");
	}

	while (0 < (len = fread(buf, 1, BUFSIZE, fp))) {
		retval = send(sock, buf, len, 0);
		if (retval == SOCKET_ERROR) {
			err_display("send()");
		}
		if (BUFSIZE > retval) {
			break;
		}
		printf("[TCP Ŭ���̾�Ʈ] %d+%d����Ʈ�� "
			"���½��ϴ�.\n", sizeof(void*), retval);
	}
	*/

	// fclose
	fclose(fp);
	
	// closesocket()
	closesocket(sock);

	// ���� ����
	WSACleanup();
	return 0;
}