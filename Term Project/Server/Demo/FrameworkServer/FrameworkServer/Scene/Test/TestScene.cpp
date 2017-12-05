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
	
	return(true);
}

bool CTestScene::OnCreate(wstring && tag, CWarp2DFramework * pFramework)
{
	if (!Base::OnCreate(std::move(tag), pFramework)) return false;
	auto rcClient = m_pFramework->GetClientSize();



	return true;
}

void CTestScene::Update(float fTimeElapsed)
{
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

	m_Player.GetCoord();
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
		auto item = make_unique<CItem>(retCoord);
		m_lstBoom.push_back(move(item));
	}

}
