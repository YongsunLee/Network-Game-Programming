#pragma comment(lib, "ws2_32")
#include <winsock2.h>
#include <stdio.h>
#include <Ws2tcpip.h>

void err_display(char *msg);
void menu();
void InputDomainName(char *name);
void InputIPv4Address(IN_ADDR addr);
void InputIPv6Address(SOCKADDR_IN6 addr);

int sel;
SOCKADDR_IN6 addr6;
DWORD ip6addrlen;
char ipv6[256];			// ipv6

int main(int argc, char *argv[])
{
	WSADATA wsa;
	
	char domain[256];		// ������ ����
	char ipv4[256];			// ipv4
	IN_ADDR addr;			// IP ����
	

	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) return 1;

	while (1) {
		menu();

		switch (sel)
		{
		// ������
		case 1:
			printf("�������� �Է��� �ּ��� : ");
			scanf("%s", &domain);

			InputDomainName(domain);

			break;
		// IPv4
		case 2:
			printf("IP�ּҸ� �Է��� �ּ���(ipv4) : ");
			scanf("%s", &ipv4);

			// 2������ ��ȯ ��
			// ��Ʈ��ũ ����Ʈ ���ķε� ��ȯ
			// addr�� union�� ���� S_addr(u_long)	
			addr.S_un.S_addr = inet_addr(ipv4);

			InputIPv4Address(addr);

			break;
		// IPv6
		case 3:
			/*
			//printf("IP�ּҸ� �Է��� �ּ���(ipv6) : ");
			//scanf("%s", &ipv6);

			//ip6addrlen = sizeof(ipv6);
			//WSAAddressToString((SOCKADDR*)&addr6, sizeof(addr6), NULL, ipv6, &ip6addrlen);

			//InputIPv6Address(addr6);
			*/

			exit(1);
			break;
		default:
			break;
		}
	}

	WSACleanup();
	return 0;
}

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

void menu()
{

	while (1) {
		printf("-------------------------------------\n");
		printf("1. ������\n");
		printf("2. IPv4\n");
		printf("3. IPv6\n");
		printf("-------------------------------------\n");

		scanf("%d", &sel);

		if (sel == 1 || sel == 2 || sel == 3) break;
	}
}

void InputDomainName(char *name)
{
	//gethostbyname �Լ��� �׻� IPv4 �ּ������� �����Ѵ�.
	// IPv6���� �����Ϸ��� addrinfo()�Լ��� ����ؾ� �Ѵ�.
	HOSTENT *ptr = gethostbyname(name);

	// NULL ���̸� ���� ����� ����
	if (ptr == NULL) {
		err_display("gethostbyname()");
	}

	// ��Ī ���
	if (ptr->h_aliases[0] != NULL) {
		for (int i = 0; ptr->h_aliases[i] != NULL; ++i) {
			printf("��Ī : %s\n", ptr->h_aliases[i]);
		}
	}
	else
		printf("��Ī : ����!\n");

	// ip�ּ� ���
	// ptr->h_addr_list �� char ��(�迭)�� in_addr*�� ����ȯ ��Ű��
	// inet_ntoa�� ���ؼ� 
	for (int i = 0; ptr->h_addr_list[i] != NULL; ++i) {
		printf("IP Addr : %s\n", inet_ntoa(*(in_addr*)ptr->h_addr_list[i]));
	}
}

void InputIPv4Address(IN_ADDR addr)
{
	HOSTENT *ptr = gethostbyaddr((char *)&addr, sizeof(addr), AF_INET);

	if (ptr == NULL) {
		err_display("gethostbyaddr()");
		exit(1);
	}

	// IPv4 ����
	if (ptr->h_addrtype != AF_INET) exit(1);

	// ���� �Էµ� IPv4�� ������ �̸�
	printf("������ �ּ�: %s\n", ptr->h_name);

	// ��Ī�� �ִ°�?
	if (ptr->h_aliases[0] != NULL) {
		for (int i = 0; ptr->h_aliases[i] != NULL; ++i) {
			printf("��Ī : %s\n", ptr->h_aliases[i]);
		}
	}
	else
		printf("��Ī : ����!\n");
	
	// ���� ��ȯ�� �������� ���� IP �ּ�
	// naver.com�� �������� �ּҸ� ������ �ݸ鿡
	// ���⼭ �ϳ��� ���� ������ ������
	// ������ ���� �ٸ��� ���� �� �տ� �ٸ��� �پ �����°� ������
	// �� ���� �ϳ��� �ּҴ� �Էµ� ���� ������ �ȴ�.
	for (int i = 0; ptr->h_addr_list[i] != NULL; ++i) {
		printf("IP Addr : %s\n", inet_ntoa(*(in_addr*)ptr->h_addr_list[i]));
	}
}

void InputIPv6Address(SOCKADDR_IN6 addr)
{
	HOSTENT *ptr = gethostbyaddr((char *)&addr, sizeof(addr), AF_INET6);

	if (ptr == NULL) {
		err_display("gethostbyaddr()");
		exit(1);
	}

	// IPv6 �� �ƴϸ� ��
	if (ptr->h_addrtype != AF_INET6) exit(1);

	// IPv6�� �مf������ ������ �ּ�
	printf("������ �ּ�: %s\n", ptr->h_name);

	// ��Ī
	for (int i = 0; ptr->h_aliases[i] != NULL; ++i) {
		printf("��Ī : %s\n", ptr->h_aliases[i]);
	}

	// �� ���� �ּҵ��� �ִ°�?
	for (int i = 0; ptr->h_addr_list[i] != NULL; ++i) {
		printf("IP Addr : %s\n", inet_ntoa(*(in_addr*)ptr->h_addr_list[i]));
	}
}