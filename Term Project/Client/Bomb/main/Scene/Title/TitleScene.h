#pragma once
#include "Camera/Camera.h"
#include "Object/Item/Item.h"
#include "Object/Player/Player.h"
#include "Scene/Test/TestScene.h"
#include "Scene/Scene.h"

class CTitleScene
	: public CScene
{
public:
	using Base = CScene;
public:
	CTitleScene();
	~CTitleScene();

	bool OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)	override;
	bool OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)		override;
	bool OnProcessingWindowMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)	override;

	bool OnCreate(wstring&& tag, CWarp2DFramework* pFramework) override;
	void Update(float fTimeElapsed) override;
	void Draw(ID2D1HwndRenderTarget * pd2dRenderTarget) override;

private:
	ComPtr<ID2D1Bitmap1>			m_bmpImage;
	D2D_RECT_F						m_rcSize;
	D2D_POINT_2F					m_ptPoisition;
	CCamera							m_Camera;
};