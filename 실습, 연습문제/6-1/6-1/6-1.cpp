// 6-1.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//
#include "stdafx.h"

struct Point3D
{
	int x;
	int y;
	int z;
};

DWORD WINAPI MyThread(LPVOID arg) 
{
	Point3D *pt = (Point3D *)arg;
	while (1) {
		printf("Running MyThread() %d : %d, %d, %d\n", GetCurrentThreadId(), pt->x, pt->y, pt->z);
		Sleep(1000);
	}
	return 0;
}

int main(int arg, char *argv[])
{
	Point3D pt1 = {10, 20, 30};
	HANDLE hThread1 = CreateThread(NULL, 0, MyThread, &pt1, 0, NULL);
	if (hThread1 == NULL) return 1;
	CloseHandle(hThread1);			// closeHandle 실패시 0값
									// 핸들을 닫는다고 해서 스레드가 종료되지는 않는다.
									// 스레드 핸들을 잃어버렸으므로 스레드를 제어할 수 없게된다.

	Point3D pt2 = { 40, 50, 60 };
	HANDLE hThread2 = CreateThread(NULL, 0, MyThread, &pt2, 0, NULL);
	if (hThread2 == NULL) return 1;
	CloseHandle(hThread2);

	while (1) {
		printf("Running main() %d\n", GetCurrentThreadId());
		Sleep(1000);
	}

    return 0;
}

