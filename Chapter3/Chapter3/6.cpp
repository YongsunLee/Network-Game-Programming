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
	
	char domain[256];		// 도메인 변수
	char ipv4[256];			// ipv4
	IN_ADDR addr;			// IP 변수
	

	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) return 1;

	while (1) {
		menu();

		switch (sel)
		{
		// 도메인
		case 1:
			printf("도메인을 입력해 주세요 : ");
			scanf("%s", &domain);

			InputDomainName(domain);

			break;
		// IPv4
		case 2:
			printf("IP주소를 입력해 주세요(ipv4) : ");
			scanf("%s", &ipv4);

			// 2진수로 전환 후
			// 네트워크 바이트 정렬로도 변환
			// addr에 union을 들어가서 S_addr(u_long)	
			addr.S_un.S_addr = inet_addr(ipv4);

			InputIPv4Address(addr);

			break;
		// IPv6
		case 3:
			/*
			//printf("IP주소를 입력해 주세요(ipv6) : ");
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
		printf("1. 도메인\n");
		printf("2. IPv4\n");
		printf("3. IPv6\n");
		printf("-------------------------------------\n");

		scanf("%d", &sel);

		if (sel == 1 || sel == 2 || sel == 3) break;
	}
}

void InputDomainName(char *name)
{
	//gethostbyname 함수는 항상 IPv4 주소정보만 리턴한다.
	// IPv6까지 리턴하려면 addrinfo()함수를 사용해야 한다.
	HOSTENT *ptr = gethostbyname(name);

	// NULL 값이면 오류 출력후 종료
	if (ptr == NULL) {
		err_display("gethostbyname()");
	}

	// 별칭 출력
	if (ptr->h_aliases[0] != NULL) {
		for (int i = 0; ptr->h_aliases[i] != NULL; ++i) {
			printf("별칭 : %s\n", ptr->h_aliases[i]);
		}
	}
	else
		printf("별칭 : 없음!\n");

	// ip주소 출력
	// ptr->h_addr_list 의 char 값(배열)을 in_addr*로 형변환 시키고
	// inet_ntoa를 통해서 
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

	// IPv4 인지
	if (ptr->h_addrtype != AF_INET) exit(1);

	// 현재 입력된 IPv4의 도메인 이름
	printf("도메인 주소: %s\n", ptr->h_name);

	// 별칭이 있는가?
	if (ptr->h_aliases[0] != NULL) {
		for (int i = 0; ptr->h_aliases[i] != NULL; ++i) {
			printf("별칭 : %s\n", ptr->h_aliases[i]);
		}
	}
	else
		printf("별칭 : 없음!\n");
	
	// 지금 변환된 도메인을 가진 IP 주소
	// naver.com이 여러가지 주소를 가지는 반면에
	// 여기서 하나의 값이 나오는 이유는
	// 도메인 값이 다르기 때문 막 앞에 다른거 붙어서 나오는거 때문에
	// 그 값은 하나라서 주소는 입력된 값만 나오게 된다.
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

	// IPv6 가 아니면 펑
	if (ptr->h_addrtype != AF_INET6) exit(1);

	// IPv6를 바꿧을때의 도메인 주소
	printf("도메인 주소: %s\n", ptr->h_name);

	// 별칭
	for (int i = 0; ptr->h_aliases[i] != NULL; ++i) {
		printf("별칭 : %s\n", ptr->h_aliases[i]);
	}

	// 그 외의 주소들이 있는가?
	for (int i = 0; ptr->h_addr_list[i] != NULL; ++i) {
		printf("IP Addr : %s\n", inet_ntoa(*(in_addr*)ptr->h_addr_list[i]));
	}
}