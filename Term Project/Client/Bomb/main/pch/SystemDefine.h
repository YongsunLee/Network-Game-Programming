#pragma once



constexpr LONG CLIENT_WIDTH = 480;
constexpr LONG CLIENT_HEIGHT = 480;

constexpr LONG CLIENT_MINIMUM_WIDTH = 480;
constexpr LONG CLIENT_MINIMUM_HEIGHT = 480;

constexpr float g_fTileWidth = 20.f;
constexpr float g_fTileHeight = 20.f;
constexpr D2D_RECT_F g_rcItemRect{ -g_fTileWidth * 0.5f, -g_fTileHeight * 0.5f, g_fTileWidth * 0.5f, g_fTileHeight * 0.5f };
constexpr D2D_RECT_F g_rcWinRect{ -g_fTileWidth *2, -g_fTileHeight * 2, g_fTileWidth * 2, g_fTileHeight * 2 };
constexpr D2D_RECT_F g_rcPlayermRect{ -g_fTileWidth * 0.4f, -g_fTileHeight * 0.4f, g_fTileWidth * 0.4f, g_fTileHeight * 0.4f };

// ĸ�� FPS ��� ���� -------------------
// �׻� ĸ�ǿ� FPS�� ���		(0 : ��Ȱ�� | 1 : Ȱ��)
#define USE_CAPTIONFPS_ALWAYS	 1
#define USE_TITLE_STRING		 1
#define USE_LIMIT_MAX_FPS		 1
#define USE_DEBUG_WINDOW		 0

#if USE_CAPTIONFPS_ALWAYS
#define SHOW_CAPTIONFPS 
#elif _DEBUG	// Debug������ �׻� ����
#define SHOW_CAPTIONFPS 
#endif

//#if defined(SHOW_CAPTIONFPS)
#define MAX_UPDATE_FPS 1.0 / 3.0
//#endif

// �ִ� FPS
#if USE_LIMIT_MAX_FPS
#define MAX_FPS 1.0 / 120.0
#else
#define MAX_FPS 0.0
#endif

#define TITLE_MAX_LENGTH 64

#if USE_TITLE_STRING
#define TITLESTRING	TEXT("Warp2D")
#endif

#define DEBUG_CLIENT_WIDTH	500
#define DEBUG_CLIENT_HEIGHT	500
