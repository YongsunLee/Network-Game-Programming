

#define WIN32_LEAN_AND_MEAN             // ���� ������ �ʴ� ������ Windows ������� �����մϴ�.
// Windows ��� ����:
#include <windows.h>

// C ��Ÿ�� ��� �����Դϴ�.
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <WinSock2.h>
#pragma comment(lib, "Ws2_32.lib")
#include <stdio.h>

// C++ ��Ÿ�� ��� �����Դϴ�.
#include <iostream>
#include <vector>
#include <list>
#include <algorithm>
#include <random>
#include <chrono>
#include <memory>
//#include <functional>
#include <string>
using namespace std;

#include <filesystem>
using namespace experimental::filesystem;

// ComPtr
#include <wrl.h>
using namespace Microsoft::WRL;

// Direct2D
#include <d2d1_3.h>
#include <dwrite_3.h>
#include <wincodec.h>
using namespace D2D1;

//// Bitmap
//#include "D2DLoadBitmap.h"
//#include "D2D_Operator.h"
//using namespace D2DBitmap;
//
//// TODO: ���α׷��� �ʿ��� �߰� ����� ���⿡�� �����մϴ�.
//#include "SystemDefine.h"
//#include "inlineFuncs.h"
//
//constexpr D2D_POINT_2F GetPositionByCoord(D2D_SIZE_U sz) noexcept { return D2D_POINT_2F{ sz.width * g_fTileWidth, sz.height * g_fTileHeight }; }

enum Colide {
	fail = -1,
	top = 0,
	bottom = 1,
	left = 2,
	right = 3,
};

#define MAZE_SIZE 12

struct Map
{
	typedef enum {
		Path = 0,
		Wall,
	}Structure;

	const int m_Width = MAZE_SIZE;
	const int m_Length = MAZE_SIZE;

	unsigned char m_ppMap[MAZE_SIZE][MAZE_SIZE] = {
		{0,0,0,0,0,0,0,0,0,0,0,0},
		{0,1,1,1,0,0,0,0,1,1,1,0},
		{0,1,0,0,0,0,0,0,0,0,1,0},
		{0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,1,1,0,0,0,0,0},
		{0,1,0,0,1,1,1,1,0,0,1,0},
		{0,1,0,0,1,1,1,1,0,0,1,0},
		{0,0,0,0,0,1,1,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0},
		{0,1,0,0,0,0,0,0,0,0,1,0},
		{0,1,1,1,0,0,0,0,1,1,1,0},
		{0,0,0,0,0,0,0,0,0,0,0,0}
	};

};

// Ÿ��Ʋ ȭ�� ���� �޽���
struct ClientPlugInMsg {
	bool StatMsg;
};

// Ÿ��Ʋ ȭ�鿡�� �޽��� ������ ���� (����)
struct ServerConnectMsg {
	enum ServerStatus {Wait, Start};
	int ClientID;			// Ŭ���̾�Ʈ ���̵�
	int PlayerCnt;			// ���� ���� Ŭ���̾�Ʈ �ο���
	ServerStatus status;	// ���� ���� ���� (��� or ���� :: Ŭ�󿡼� Scene ����)
};

// Ŭ���̾�Ʈ���� ���� �޽���
struct ClientSendMsg {
	enum PlayerDir {Left, Right, Up, Down, LeftUp, LeftDown, RightUp, RightDown};
	int ClientID;	// Ŭ���̾�Ʈ�� ID
	int BombCnt;	// ���� ��ź ����
	bool DropBomb;	// ��ź ��ġ �޽���
};

// �÷��̾� ����ü
struct Player {
	enum PlayerDir { Left, Right, Up, Down, LeftUp, LeftDown, RightUp, RightDown };
	enum PlayerStatus { Living, Death };
	D2D1_POINT_2F position;
	PlayerDir dir;
	PlayerStatus status;
};

// ��ź ����ü
struct Bomb {
	enum BombStatus { Drop, Explosion };
	D2D1_POINT_2F position;
	BombStatus status;
};

// ���� ����
enum GameStatus { Playing, GameOver };

// �������� ���� �޽��� ����ü (Ŭ�󿡼� ���� �޽��� ����ü)
struct ServerMsg {
	int Map[12][12];
	Bomb bomb;
	Player player;
	GameStatus gameStatus;
};

// �׽�Ʈ�� ����ü
struct ClientMsg {
	int CheckData;
	//D2D_POINT_2F Dir;
};
