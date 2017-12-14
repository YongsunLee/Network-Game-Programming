#include "stdafx.h"

#include "Framework/Warp2DFramework.h"
#include "Framework/IndRes/IndRes.h"

#include "TestScene.h"


CTestScene::CTestScene()
	: m_Player{ SizeU(0, 0) }, m_Player2{ SizeU(11, 0) }
{
}

CTestScene::~CTestScene()
{
	while (m_lstBlock.empty()) {
		m_lstBlock.pop_front();
	}
	m_lstBlock.clear();
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
		case 'A':
			m_f2Move.x = -1;
		
		break;
		case 'W':
			m_f2Move.y=-1;

			break;
		case 'D':
			m_f2Move.x=1;

			break;
		case 'S':
			m_f2Move.y=1;

			break;
		case VK_SPACE:
			m_bBomb = true;
			break;
		case VK_ESCAPE:
			PostQuitMessage(0);
			break;
		}
		break;
	case WM_KEYUP:
		switch (wParam)
		{
		case 'A':
			if (m_f2Move.x < 0)
				m_f2Move.x = 0;
			break;
		case 'W':
			if (m_f2Move.y< 0)
				m_f2Move.y=0;
			break;
		case 'D':
			if (m_f2Move.x > 0)
				m_f2Move.x=0;
			break;
		case 'S':
			if (m_f2Move.y> 0)
				m_f2Move.y= 0;
			break;

		default:
			return false;
		}
		break;
	default:
		return false;
	}
	return(true);
}

bool CTestScene::OnCreate(wstring && tag, CWarp2DFramework * pFramework)
{
	if (!Base::OnCreate(std::move(tag), pFramework)) return false;

	m_pNetwork = make_unique<CNetwork>();
	m_pNetwork.get()->OnCreate();

	auto rcClient = m_pFramework->GetClientSize();
	m_Camera.SetClientSize(Point2F(rcClient.right, rcClient.bottom));

	auto rendertarget = m_pFramework->GetRenderTarget();

	rendertarget->CreateSolidColorBrush(ColorF{ ColorF::Crimson }, &m_pd2dsbrDefault);
	rendertarget->CreateSolidColorBrush(ColorF{ ColorF::MistyRose }, &m_pd2dsbrTileA);
	rendertarget->CreateSolidColorBrush(ColorF{ ColorF::LightPink }, &m_pd2dsbrTileB);

	m_Camera.SetPosition(D2D_POINT_2F{ g_fTileWidth * 11 / 2, g_fTileWidth * 11 / 2 });
	m_Camera.SetAnchor(Point2F(0.f, 0.f));

	m_Player.RegisterImage(m_pIndRes.get(), rendertarget.Get(), "Graphics/player.png"s, SizeU(4, 4));
	m_Player2.RegisterImage(m_pIndRes.get(), rendertarget.Get(), "Graphics/goblin.png"s, SizeU(4, 4));

	m_Player.RegisterWinImage(m_pIndRes.get(), rendertarget.Get(), "Graphics/Icon/Win.png"s);
	m_Player2.RegisterWinImage(m_pIndRes.get(), rendertarget.Get(), "Graphics/Icon/Win.png"s);
	
	m_Bombs[0].RegisterImage(m_pIndRes.get(), rendertarget.Get(), "Graphics/Icon/BombPng.png"s);
	m_Bombs[0].RegisterBoomImage(m_pIndRes.get(), rendertarget.Get(), "Graphics/Icon/Boom.png"s);
	
	
	
	
	for (int i = 0; i < 20; ++i) {
		m_Bombs[i].RegisterImage(m_Bombs[0].GetBitmap());
		m_Bombs[i].RegisterBoomImage(m_Bombs[0].GetBoomBitmap());
	}

	for (int i = 0; i < 12; ++i)
		for (int j = 0; j < 12; ++j)
		{
			if (m_Map.m_ppMap[i][j] == 1) {
				auto block = make_unique<CBlock>(SizeU(i, j));

				block->RegisterImage(m_pIndRes.get(), rendertarget.Get(),
					"Graphics/Icon/Icon Frame.png");
				m_lstBlock.push_back(move(block));
			}
		}
	return true;
}

void CTestScene::Update(float fTimeElapsed)
{

	//m_Camera.SetPosition(m_Player.GetPosition());
	m_pNetwork->MakeMsg(m_f2Move,m_bBomb);
	///////////////////////////////////////////////////
	
	m_nBombCnt = m_pNetwork->GetBombCnt();
	for (int i = 0; i < m_nBombCnt; ++i) {
		m_Bombs[i].SetPosition(m_pNetwork->GetBombPos(i));
		m_Bombs[i].SetBoomState(m_pNetwork->GetBombState(i));
	}
	m_bBomb = false;
	//////////////////////////////////////////////////
	m_Player.SetPosition(m_pNetwork->GetPosition(0));
	m_Player.SetDir(m_pNetwork->GetMove(0));
	m_Player.SetActive(m_pNetwork->GetActive(0));

	m_Player2.SetPosition(m_pNetwork->GetPosition(1));
	m_Player2.SetDir(m_pNetwork->GetMove(1));
	m_Player2.SetActive(m_pNetwork->GetActive(1));

	/////////////////////////////////////////////////

	bool gameover = false;
	if (!m_Player.GetActive() || !m_Player2.GetActive()) gameover = true;

	if (m_Player.GetActive() && m_Player2.GetActive()) {
		gameover = false;
		m_Player.win = false;
		m_Player2.win = false;
	}
	if (gameover) {
		m_Player.win = m_Player.GetActive();
		m_Player2.win = m_Player2.GetActive();
	}


	m_Player.Update(fTimeElapsed);
	m_Player2.Update(fTimeElapsed);
}

void CTestScene::Draw(ID2D1HwndRenderTarget * pd2dRenderTarget)
{
	auto cameramtx = m_Camera.RegenerateViewMatrix();
	pd2dRenderTarget->SetTransform(cameramtx);

	for (int x = 0; x < 12; ++x)
		for (int y = 0; y < 12; ++y)
			pd2dRenderTarget->FillRectangle(
				GetPositionByCoord(SizeU(x, y)) + g_rcItemRect
				, (x + y) % 2 ? m_pd2dsbrTileA.Get() : m_pd2dsbrTileB.Get());

	for (const auto& p : m_lstBlock)
		p->Draw(pd2dRenderTarget);

	for(int i=0 ; i<m_nBombCnt ; ++i)
		m_Bombs[i].Draw(pd2dRenderTarget);

	m_Player.Draw(pd2dRenderTarget);
	m_Player2.Draw(pd2dRenderTarget);

}

void CTestScene::MakeBoom()
{
	D2D1_SIZE_U retCoord = m_Player.GetCoord();

	bool col = false;
	for (auto& p : m_lstBlock) {
		if (p->GetCoord() == retCoord) {
			col = true;
			break;
		}
	}
	if (!col)m_bBomb = true;

}
