

#define WIN32_LEAN_AND_MEAN             // 거의 사용되지 않는 내용은 Windows 헤더에서 제외합니다.
// Windows 헤더 파일:
#include <windows.h>

// C 런타임 헤더 파일입니다.
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <WinSock2.h>
#pragma comment(lib, "Ws2_32.lib")
#include <stdio.h>

// C++ 런타임 헤더 파일입니다.
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
//// TODO: 프로그램에 필요한 추가 헤더는 여기에서 참조합니다.
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

// 타이틀 화면 접속 메시지
struct ClientPlugInMsg {
	bool StatMsg;
};

// 타이틀 화면에서 메시지 받은거 답장 (서버)
struct ServerConnectMsg {
	enum ServerStatus {Wait, Start};
	int ClientID;			// 클라이언트 아이디
	int PlayerCnt;			// 현재 접속 클라이언트 인원수
	ServerStatus status;	// 현재 게임 상태 (대기 or 시작 :: 클라에서 Scene 변경)
};

// 클라이언트에서 보낼 메시지
struct ClientSendMsg {
	enum PlayerDir {Left, Right, Up, Down, LeftUp, LeftDown, RightUp, RightDown};
	int ClientID;	// 클라이언트의 ID
	int BombCnt;	// 현재 폭탄 개수
	bool DropBomb;	// 폭탄 설치 메시지
};

// 플레이어 구조체
struct Player {
	enum PlayerDir { Left, Right, Up, Down, LeftUp, LeftDown, RightUp, RightDown };
	enum PlayerStatus { Living, Death };
	D2D1_POINT_2F position;
	PlayerDir dir;
	PlayerStatus status;
};

// 폭탄 구조체
struct Bomb {
	enum BombStatus { Drop, Explosion };
	D2D1_POINT_2F position;
	BombStatus status;
};

// 게임 상태
enum GameStatus { Playing, GameOver };

// 서버에서 보낼 메시지 구조체 (클라에선 받을 메시지 구조체)
struct ServerMsg {
	int Map[12][12];
	Bomb bomb;
	Player player;
	GameStatus gameStatus;
};

// 테스트용 구조체
struct ClientMsg {
	int CheckData;
	//D2D_POINT_2F Dir;
};
