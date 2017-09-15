#pragma comment(lib, "ws2_32")

#include<WinSock2.h>
#include<iostream>

int main(int argc, char *argv[])
{
	// 윈도우소켓 초기화
	WSADATA wsa;
	WSADATA wsa1_1;

	// 2.2 버전
	// MAKEWORD(a, b)
	// a가 상위 b가 하위

	// 0이 성공, 실패하면 1
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
		return 1;
	}

	// 하위 비트가 주버전
	// 상위 비트가 부버전

	// Ws2_32에서 호출자가 사용할것으로 예상하는 윈도우 사양 소켓
	std::cout << "wVersion      : " << (wsa.wVersion & 0x000f)  << "." << ((wsa.wVersion >> 8 ) & 0x00ff ) << std::endl;

	// Ws2_32가 지원할수 있는 최대 소켓 사양
	std::cout << "wHighVersion  : " << (wsa.wHighVersion & 0x000f) << "." << ((wsa.wHighVersion >> 8) & 0x00ff ) << std::endl;

	// Ws2_32가 소켓 구현에 대한 설명을 복사하는 NULL로 끝나는 문자열
	std::cout << "szDescription : " << wsa.szDescription << std::endl;

	// Ws2_32이 관련상태 또는 구성 정보를 복사하는 NULL으로 끝나는 문자열
	std::cout << "szSystemStatus: " << wsa.szSystemStatus << std::endl;

	std::cout << std::endl;

	// 1.1 버전
	if (WSAStartup(MAKEWORD(1, 1), &wsa1_1) != 0) {
		return 1;
	}

	std::cout << "wVersion      : " << (wsa1_1.wVersion & 0x000f) << "." << ((wsa1_1.wVersion >> 8 ) & 0x00ff ) << std::endl;
	std::cout << "wHighVersion  : " << (wsa1_1.wHighVersion & 0x00f) << "." << ((wsa1_1.wHighVersion >> 8) & 0x00ff ) << std::endl;
	std::cout << "szDescription : " << wsa1_1.szDescription << std::endl;
	std::cout << "szSystemStatus: " << wsa1_1.szSystemStatus << std::endl;

	MessageBox(NULL, "윈속 초기화 성공", "알림", MB_OK);

	// 윈도우 소켓 종료
	WSACleanup();

	return 0;
}