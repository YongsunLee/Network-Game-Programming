#include "stdafx.h"

#include "Framework/Warp2DFramework.h"
#include "Framework/IndRes/IndRes.h"

#include "TestScene.h"

#pragma warning (disable : 4996)

CTestScene::CTestScene()
	: m_Player { SizeU(25, 25) },
	  m_Enemy  { SizeU(15, 15) }
{
}


CTestScene::~CTestScene()
{
}

bool CTestScene::OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	switch (nMessageID)
	{
	case WM_LBUTTONDOWN:	
	case WM_MBUTTONDOWN:	
	case WM_RBUTTONDOWN:	
	case WM_MOUSEMOVE:		
	case WM_LBUTTONUP:		
	case WM_MBUTTONUP:		
	case WM_RBUTTONUP:		
	case WM_MOUSEWHEEL:		
		break;
	default:
		return false;
	}

	return(true);
}

bool CTestScene::OnProcessingWindowMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	switch (nMessageID)
	{
	case WM_SIZE:
		{
			auto rcClient = m_pFramework->GetClientSize();
			m_Camera.SetClientSize(Point2F(rcClient.right, rcClient.bottom));
		}
		break;

	case WM_LBUTTONDOWN:
	case WM_MBUTTONDOWN:
	case WM_RBUTTONDOWN:
	case WM_MOUSEMOVE:
	case WM_LBUTTONUP:
	case WM_MBUTTONUP:
	case WM_RBUTTONUP:
	case WM_MOUSEWHEEL:
		return OnProcessingMouseMessage(hWnd, nMessageID, wParam, lParam);

	case WM_KEYDOWN:
	case WM_KEYUP:
	case WM_CHAR:
		return OnProcessingKeyboardMessage(hWnd, nMessageID, wParam, lParam);

	default:
		return false;
	}

	return true;
}

bool CTestScene::OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	switch (nMessageID)
	{
	case WM_KEYDOWN:
		switch (wParam)
		{
		case 'A':		m_Player.Move(CPlayer::Dir::left); m_Msg.CheckData = CPlayer::Dir::left;
			break;
		case 'W':		m_Player.Move(CPlayer::Dir::top); m_Msg.CheckData = CPlayer::Dir::top;
			break;
		case 'D':		m_Player.Move(CPlayer::Dir::right); m_Msg.CheckData = CPlayer::Dir::right;
			break;
		case 'S':		m_Player.Move(CPlayer::Dir::bottom); m_Msg.CheckData = CPlayer::Dir::bottom;
			break;
		case 'Z':		m_Camera.Scale(m_Camera.GetScale() * 2.f);
			break;
		case 'X':		m_Camera.Scale(m_Camera.GetScale() * 0.5f);
			break;
		default:
			return false;
		}
		break;
	default:
		return false;
	}
	AllocConsole();
	freopen("CONOUT$", "wt", stdout);
	// 키입력 이후 전송
	m_TCP.ClientSend(m_Msg);
	return(true);
}

bool CTestScene::OnCreate(wstring && tag, CWarp2DFramework * pFramework)
{
	if (!Base::OnCreate(std::move(tag), pFramework)) return false;

	auto rcClient = m_pFramework->GetClientSize();
	m_Camera.SetClientSize(Point2F(rcClient.right, rcClient.bottom));

	auto rendertarget = m_pFramework->GetRenderTarget();

	// 타일맵
	rendertarget->CreateSolidColorBrush(ColorF{ ColorF::Crimson }, &m_pd2dsbrDefault);
	rendertarget->CreateSolidColorBrush(ColorF{ ColorF::MistyRose }, &m_pd2dsbrTileA);
	rendertarget->CreateSolidColorBrush(ColorF{ ColorF::LightPink }, &m_pd2dsbrTileB);

	// 카메라
	m_Camera.SetPosition(m_Player.GetPosition());
	m_Camera.SetAnchor(Point2F(0.f, 0.f));

	// 플레이어
	m_Player.RegisterImage(m_pIndRes.get(), rendertarget.Get(), "Graphics/player.png", SizeU(4,4));

	// 적
	m_Enemy.RegisterImage(m_pIndRes.get(), rendertarget.Get(), "Graphics/player.png", SizeU(4, 4));

	return true;
}

void CTestScene::Update(float fTimeElapsed)
{
	// 서버에서 보낸거 받고
	m_TCP.ClientRecive();

	m_Camera.SetPosition(m_Player.GetPosition());

	m_Player.Update(fTimeElapsed);
	m_Enemy.Update(fTimeElapsed);
}

void CTestScene::Draw(ID2D1HwndRenderTarget * pd2dRenderTarget)
{
	auto cameramtx = m_Camera.RegenerateViewMatrix();
	pd2dRenderTarget->SetTransform(cameramtx);
	
	// 타일맵 각 배열 좌표
	// 0, 0 번째 타일의 좌표를 계산하는 구조
	for (int x = 0; x < 50; ++x)
		for (int y = 0; y < 50; ++y)
			pd2dRenderTarget->FillRectangle(
				  GetPositionByCoord(SizeU(x,y)) + g_rcItemRect
				, (x + y) % 2 ? m_pd2dsbrTileA.Get() : m_pd2dsbrTileB.Get());

	m_Player.Draw(pd2dRenderTarget);
	m_Enemy.Draw(pd2dRenderTarget);
}
