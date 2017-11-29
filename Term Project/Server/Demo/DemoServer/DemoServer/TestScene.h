#pragma once

#include "../DemoServer/Scene.h"
#include "../DemoServer/Player.h"
#include "../DemoServer/Item.h"
#include "../DemoServer/Block.h"

class CTestScene :
	public CScene
{
public:
	using Base = CScene;
public:
	CTestScene();
	~CTestScene();
	bool OnCreate(wstring&& tag) override;
	
	void MakeBoom();
	
	void Update(float fTimeElapsed) override;



	float GetLength(D2D_POINT_2F sz) { return sqrt((sz.x*sz.x) + (sz.y*sz.y)); }
private:
	Map								m_Map;

	CPlayer							m_Player;

	list<unique_ptr<CBlock>>		m_lstBlock;
	list<unique_ptr<CItem>>			m_lstBoom;
};
