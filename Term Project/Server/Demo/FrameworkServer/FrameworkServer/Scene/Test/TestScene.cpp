#include "stdafx.h"

#include "Framework/Warp2DFramework.h"
#include "TestScene.h"


CTestScene::CTestScene()
	: m_Player{ SizeU(0, 0) }
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
		case 'A':
		{
			m_Player.SetMoveX(-1);
			m_Player.SetDir(CPlayer::Dir::left);
		}
		break;
		case 'W':
			m_Player.SetMoveY(-1);
			m_Player.SetDir(CPlayer::Dir::top);
			break;
		case 'D':
			m_Player.SetMoveX(1);
			m_Player.SetDir(CPlayer::Dir::right);

			break;
		case 'S':
			m_Player.SetMoveY(1);
			m_Player.SetDir(CPlayer::Dir::bottom);
			break;
		case VK_SPACE:
			MakeBoom();
			break;
		}
		break;
		
	case WM_KEYUP:
		switch (wParam)
		{
		case 'A':
			if (m_Player.GetMoveX() < 0)
				m_Player.SetMoveX(0);
			break;
		case 'W':
			if (m_Player.GetMoveY() < 0)
				m_Player.SetMoveY(0);
			break;
		case 'D':
			if (m_Player.GetMoveX() > 0)
				m_Player.SetMoveX(0);
			break;
		case 'S':
			if (m_Player.GetMoveY() > 0)
				m_Player.SetMoveY(0);
			break;

		default:
			return false;
		}
		break;
		
	default:
		return false;
	}
	m_Msg.Dir = m_Player.GetMove();
	
	
	return(true);
}

bool CTestScene::OnCreate(wstring && tag, CWarp2DFramework * pFramework)
{
	if (!Base::OnCreate(std::move(tag), pFramework)) return false;

	auto rcClient = m_pFramework->GetClientSize();
	m_Camera.SetClientSize(Point2F(rcClient.right, rcClient.bottom));

	auto rendertarget = m_pFramework->GetRenderTarget();

	rendertarget->CreateSolidColorBrush(ColorF{ ColorF::Crimson }, &m_pd2dsbrDefault);
	rendertarget->CreateSolidColorBrush(ColorF{ ColorF::MistyRose }, &m_pd2dsbrTileA);
	rendertarget->CreateSolidColorBrush(ColorF{ ColorF::LightPink }, &m_pd2dsbrTileB);

	m_Camera.SetPosition(D2D_POINT_2F{ g_fTileWidth * 11 / 2, g_fTileWidth * 11 / 2 });
	//m_Player.GetPosition());
	m_Camera.SetAnchor(Point2F(0.f, 0.f));

	m_Player.RegisterImage(m_pIndRes.get(), rendertarget.Get(), "Graphics/player.png", SizeU(4, 4));
	m_Player.RegisterImage(m_pIndRes.get(), rendertarget.Get(), "Graphics/player.png", SizeU(4, 4));

	uniform_int_distribution<> pos_random{ 0,11 };
	default_random_engine reng(random_device{}());

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
	for (auto& p : m_lstBlock) {
		auto pos = m_Player.GetPosition() + m_Player.GetSize();
		if (p->Colided(pos + m_Player.GetMove()))
		{
			m_Player.Move(-1 * m_Player.GetMove(), fTimeElapsed);
		}
	}

	for (auto& p : m_lstBoom) {
		auto pos = m_Player.GetPosition() + m_Player.GetSize();
		if (p->Colided(pos + m_Player.GetMove()))
		{
			if(GetLength(m_Player.GetPosition() - p->GetPosition())>m_Player.GetSize().bottom+p->GetSize().bottom)
			m_Player.Move(-1 * m_Player.GetMove(), fTimeElapsed);
		}
	}

	//m_Player.SetPosition(m_TCP.ClientRecive());
	//m_Player.Update(fTimeElapsed);
	m_Player.GetCoord();

	m_TCP.ClientSend(m_Msg);
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


	for (const auto& p : m_lstBoom)
		p->Draw(pd2dRenderTarget);


	m_Player.Draw(pd2dRenderTarget);

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
	if (!col) {

	auto rendertarget = m_pFramework->GetRenderTarget();
	auto item = make_unique<CItem>(retCoord);

	item->RegisterImage(m_pIndRes.get(), rendertarget.Get(), "Graphics/Icon/wonder stone.png");
	m_lstBoom.push_back(move(item));
	}

}
