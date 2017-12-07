#pragma once

#include "Scene/Scene.h"
#include "Camera/Camera.h"
#include "Object/Player/Player.h"
#include "Object/Item/Item.h"
#include "Object/Block/Block.h"
#include "UI/Inventory/Inventory.h"
#include "Network.h"


class CTestScene :
	public CScene
{
public:
	using Base = CScene;
public:
	CTestScene();
	~CTestScene();

	bool OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)	override;
	bool OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)		override;
	bool OnProcessingWindowMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)	override;

	bool OnCreate(wstring&& tag, CWarp2DFramework* pFramework) override;
	void Update(float fTimeElapsed) override;
	void Draw(ID2D1HwndRenderTarget * pd2dRenderTarget) override;
	float GetLength(D2D_POINT_2F sz) { return sqrt((sz.x*sz.x) + (sz.y*sz.y)); }
	void MakeBoom();
private:

	ComPtr<ID2D1SolidColorBrush>	m_pd2dsbrDefault	{ nullptr }	;
	ComPtr<ID2D1SolidColorBrush>	m_pd2dsbrTileA		{ nullptr }	;
	ComPtr<ID2D1SolidColorBrush>	m_pd2dsbrTileB		{ nullptr }	;


	Map								m_Map;

	D2D_POINT_2F					m_f2Move{Point2F(0,0)};

	CPlayer							m_Player;
	CPlayer							m_Player2;
	CCamera							m_Camera;

	list<unique_ptr<CBlock>>		m_lstBlock;
	CBomb							m_Bombs[144];
	bool							m_bBomb{ false };
	int								m_nBombCnt{ 1 };
	
	unique_ptr<CNetwork>				m_pNetwork;
	CUIInventory					m_uiInventory;
};
