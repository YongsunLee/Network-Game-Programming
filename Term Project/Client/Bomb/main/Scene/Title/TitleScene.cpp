#include "stdafx.h"

#include "Framework/Warp2DFramework.h"
#include "Framework/IndRes/IndRes.h"

#include "TitleScene.h"

CTitleScene::CTitleScene()
{
}

CTitleScene::~CTitleScene()
{
}

bool CTitleScene::OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	switch (nMessageID)
	{
	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_SPACE:
			break;
		default:
			return false;
		}
		break;
	case WM_KEYUP:
		switch (wParam)
		{
		case VK_SPACE:
			//m_pFramework->BuildScene<CTestScene>(L"Test"s);
			m_pFramework->ChangeScene(L"Test"s, true);
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

bool CTitleScene::OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
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

bool CTitleScene::OnProcessingWindowMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
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

bool CTitleScene::OnCreate(wstring && tag, CWarp2DFramework * pFramework)
{
	if (!Base::OnCreate(std::move(tag), pFramework)) return false;

	auto rcClient = m_pFramework->GetClientSize();
	m_Camera.SetClientSize(Point2F(rcClient.right, rcClient.bottom));

	auto rendertarget = m_pFramework->GetRenderTarget();
	
	m_Camera.SetPosition(D2D_POINT_2F{ g_fTileWidth * 11 / 2, g_fTileWidth * 11 / 2 });
	m_Camera.SetAnchor(Point2F(0.f, 0.f));

	path filename = "Graphics/title_bg.png";
	LoadImageFromFile(
		m_pIndRes.get()->wicFactory()
		, rendertarget.Get()
		, filename.c_str()
		, &m_bmpImage
	);

	m_ptPoisition = D2D_POINT_2F{-10.f, -10.f};
	m_rcSize = D2D_RECT_F{ 0, 0, CLIENT_MINIMUM_WIDTH / 2, CLIENT_MINIMUM_HEIGHT / 2 };

	return true;
}

void CTitleScene::Update(float fTimeElapsed)
{

}

void CTitleScene::Draw(ID2D1HwndRenderTarget * pd2dRenderTarget)
{
	auto cameramtx = m_Camera.RegenerateViewMatrix();
	pd2dRenderTarget->SetTransform(cameramtx);

	pd2dRenderTarget->DrawBitmap(m_bmpImage.Get(), m_rcSize + m_ptPoisition);
}
