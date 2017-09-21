#pragma comment(lib, "ws2_32")
#include <winsock2.h>
#include <stdio.h>

BOOL IsLittleEndian();
BOOL IsBigEndian();


int main(int argc, char *argv[])
{
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2,2), &wsa) != 0) return 1;

	if (IsLittleEndian() == true) printf("\n�̰��� ��Ʋ����� �Դϴ�.\n");
	if (IsBigEndian() == true) printf("\n�̰��� �򿣵�� �Դϴ�.\n");

	printf("\n");

	WSACleanup();
	return 0;
}

BOOL IsLittleEndian()
{
	u_short x1 = 0x1234;
	u_short x2;
	u_short x3;

	// ��Ʈ��ũ�� �ٲ۴�
	x2 = htons(x1);

	// ���� ȣ��Ʈ�� ����Ʈ�� ��Ʋ������̸� 
	// x1 ������ 0x3412�� ����Ǿ����� ���̱� ������ (ȭ�� ����� 1234)
	printf("%p", &x1);
	// x2���� 0x1234�� ���� ���Եȴ�. (ȭ�� ����� �д� ����� ��Ʋ������̱� ���� 3412)
	printf("%p", &x2);

	printf("\n0x%x, 0x%x", x1, x2);

	// �ΰ��� ���� �ٸ��� ��Ʋ�����
	if (x1 == x2) return false;
	else return true;
}

BOOL IsBigEndian()
{
	u_short x1 = 0x1234;
	u_short x2;
	
	// ��Ʈ��ũ�� �ٲ۴�.
	x2 = ntohs(x1);

	// �ΰ��� ���� ������ �򿣵��
	if (x1 == x2) return true;
	else return false;
}
