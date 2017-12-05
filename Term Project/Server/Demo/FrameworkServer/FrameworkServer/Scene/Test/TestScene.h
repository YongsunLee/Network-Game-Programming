#pragma once

#include "Scene/Scene.h"
#include "Object/Player/Player.h"
#include "Object/Item/Item.h"
#include "Object/Block/Block.h"

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
	float GetLength(D2D_POINT_2F sz) { return sqrt((sz.x*sz.x) + (sz.y*sz.y)); }
	void MakeBoom();

private:
	Map								m_Map;

	CPlayer							m_Player;

	list<unique_ptr<CBlock>>		m_lstBlock;
	list<unique_ptr<CItem>>			m_lstBoom;
};
