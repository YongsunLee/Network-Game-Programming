#pragma comment(lib, "ws2_32")
#include <winsock2.h>
#include <stdio.h>

BOOL IsLittleEndian();
BOOL IsBigEndian();


int main(int argc, char *argv[])
{
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2,2), &wsa) != 0) return 1;

	if (IsLittleEndian() == true) printf("\n이것은 리틀엔디안 입니다.\n");
	if (IsBigEndian() == true) printf("\n이것은 빅엔디안 입니다.\n");

	printf("\n");

	WSACleanup();
	return 0;
}

BOOL IsLittleEndian()
{
	u_short x1 = 0x1234;
	u_short x2;
	u_short x3;

	// 네트워크로 바꾼다
	x2 = htons(x1);

	// 현재 호스트의 바이트가 리틀엔디안이면 
	// x1 저장은 0x3412로 저장되어있을 것이기 때문에 (화면 출력은 1234)
	printf("%p", &x1);
	// x2에는 0x1234의 값이 들어가게된다. (화면 출력은 읽는 방식은 리틀엔디안이기 때문 3412)
	printf("%p", &x2);

	printf("\n0x%x, 0x%x", x1, x2);

	// 두개의 값이 다르면 리틀엔디안
	if (x1 == x2) return false;
	else return true;
}

BOOL IsBigEndian()
{
	u_short x1 = 0x1234;
	u_short x2;
	
	// 네트워크로 바꾼다.
	x2 = ntohs(x1);

	// 두개의 값이 같으면 빅엔디안
	if (x1 == x2) return true;
	else return false;
}
