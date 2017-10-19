// 6-1.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//
#include "stdafx.h"

DWORD WINAPI MyThread(LPVOID arg)
{
	while (1);
	return 0;
}

int main(int arg, char *argv[])
{
	SYSTEM_INFO si;
	GetSystemInfo(&si);

	for (int i = 0; i < (int)si.dwNumberOfProcessors; ++i) {
		HANDLE hThread = CreateThread(NULL, 0, MyThread, NULL, 0, NULL);
		if (hThread == NULL) return 1;

		SetThreadPriority(hThread, THREAD_PRIORITY_TIME_CRITICAL);
		CloseHandle(hThread);
	}
	Sleep(1000);
	while (1) { printf("주 스레드 실행!\n"); break; }
	return 0;
}

