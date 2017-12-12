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

#include "../Server/Timer/Timer.h"
#include "D2D_Operator.h"
#include "SystemDefine.h"
#include "SceneManager/SceneManager.h"

////////////////////////////////
#include "Object/Block/Block.h"
#include "Object/Item/Item.h"
#include "Object/Player/Player.h"
///////////////////////////////

enum Colide {
	fail = -1,
	top = 0,
	bottom = 1,
	left = 2,
	right = 3,
};

#define MAZE_SIZE 12

#define MAX_FPS 1.0 / 60.0
#define TITLE_MAX_LENGTH 64

#define MAX_UPDATE_FPS 1.0 / 3.0

struct Map
{
	typedef enum {
		Path = 0,
		Wall,
	}Structure;

	const int m_Width = MAZE_SIZE;
	const int m_Length = MAZE_SIZE;

	unsigned char m_ppMap[MAZE_SIZE][MAZE_SIZE] = {
		{ 0,0,0,0,0,0,0,0,0,0,0,0 },
		{ 0,1,1,1,0,0,0,0,1,1,1,0 },
		{ 0,1,0,0,0,0,0,0,0,0,1,0 },
		{ 0,0,0,0,0,0,0,0,0,0,0,0 },
		{ 0,0,0,0,0,1,1,0,0,0,0,0 },
		{ 0,1,0,0,1,1,1,1,0,0,1,0 },
		{ 0,1,0,0,1,1,1,1,0,0,1,0 },
		{ 0,0,0,0,0,1,1,0,0,0,0,0 },
		{ 0,0,0,0,0,0,0,0,0,0,0,0 },
		{ 0,1,0,0,0,0,0,0,0,0,1,0 },
		{ 0,1,1,1,0,0,0,0,1,1,1,0 },
		{ 0,0,0,0,0,0,0,0,0,0,0,0 }
	};

};

// 타이틀 화면 접속 메시지
struct ClientPlugInMsg {
	enum Scene{ title, main };
};

// 타이틀 화면에서 메시지 받은거 답장 (서버)
struct ServerConnectMsg {
	enum ServerStatus { Wait, Start };
	int ClientID;			// 클라이언트 아이디
	int PlayerCnt;			// 현재 접속 클라이언트 인원수
	ServerStatus status;	// 현재 게임 상태 (대기 or 시작 :: 클라에서 Scene 변경)
};

// 테스트용 구조체
struct ClientSendMsg {
	float CheckData[2];
	bool SetBomb;
};

// Msg Queue
struct QueueMsg {
	int ID;
	ClientSendMsg* msg;
};

// ServerSendMsg
struct ServerSendMsg {
	enum PlayerStatus { Living, Death };
	D2D_POINT_2F playerPos[2];
	CPlayer::Dir moveVec[2];
	PlayerStatus status[2];
	int nbombCnt;
	D2D_POINT_2F BombPos[144];
	bool BombStat[144];
};

struct ChangeSceneMsg {
	
	bool Change;
};