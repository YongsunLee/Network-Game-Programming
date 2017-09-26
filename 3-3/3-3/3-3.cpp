#pragma comment(lib, "ws2_32")
#include <WinSock2.h>
#include <stdio.h>

#define TESTNAME "www.example.com"

void err_display(char *msg);

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

// 도메인 -> IPv4
BOOL GetIPAddr(char *name, IN_ADDR *addr)
{
	HOSTENT *ptr = gethostbyname(name);
	
	// NULL 값이면 오류 출력후 종료
	if (ptr == NULL) {
		err_display("gethostbyname()");
		return false;
	}

	// addrtype이 IPv4가 아니면 종료
	if (ptr->h_addrtype != AF_INET) return false;

	// ptr->h_addr의 주소값부터 h_length 길이만큼 addr에 복사
	memcpy(addr, ptr->h_addr, ptr->h_length);

	return TRUE;
}

// IPv4 -> 도메인
BOOL GetDomainName(IN_ADDR addr, char *name, int namelen)
{
	HOSTENT *ptr = gethostbyaddr((char *)&addr, sizeof(addr), AF_INET);

	if (ptr == NULL) {
		err_display("gethostbyaddr()");
		return false;
	}

	if (ptr->h_addrtype != AF_INET) return false;

	strncpy(name, ptr->h_name, namelen);
	return true;
}

int main(int argc, char *argv[])
{
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) return 1;

	printf("도메인 이름 변환전 = %s \n", TESTNAME);

	// 도메인 이름 -> IP 주소
	IN_ADDR addr;

	if (GetIPAddr(TESTNAME, &addr))
	{
		// 성공이면 결과 출력
		printf("IP 주소(변환 후) = %s\n", inet_ntoa(addr));

		// IP주소 -> 도메인 이름
		char name[256];
		if (GetDomainName(addr, name, sizeof(name)))
			printf("도메인 이름(다시 변환 후) = %s\n", name);
	}
	
	WSACleanup();
	return 0;
}
