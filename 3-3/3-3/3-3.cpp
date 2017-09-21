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

	strncpy();
}