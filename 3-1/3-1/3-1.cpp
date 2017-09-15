#pragma comment(lib, "ws2_32")
#include <WinSock2.h>
#include <stdio.h>

int main(int argc, char *argv[]) 
{
	WSADATA wsa;

	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) return 1;

	u_short x1 = 0x1234;
	u_long y1 = 0x12345678;
	u_short x2;
	u_long y2;

	// 호스트 바이트 -> 네트워크 바이트
	printf("[호스트 바이트 -> 네트워크 바이트]\n");
	printf("0x%x -> 0x%x\n", x1, x2 = htons(x1));
	printf("0x%x -> 0x%x\n", y1, y2 = htonl(y1));

	//네트워크 바이트 -> 호스트 바이트
	printf("\n[네트워크바이트 -> 호스트 바이트]\n");
	printf("0x%x -> 0x%x\n", x2, ntohs(x2));
	printf("0x%x -> 0x%x\n", y2, ntohl(y2));

	// 잘못 사용된 예
	// htons로 하면 정상
	// x1의 변수형이 u_short 임을 감안해 보았을때 변수형에 맞추어 사용해야 되는데 long 으로 해서 잘못된 것
	printf("\n[잘못 사용 예시]\n");
	printf("0x%x -> 0x%x\n", x1, htonl(x1));

	WSACleanup();
	return 0;
}