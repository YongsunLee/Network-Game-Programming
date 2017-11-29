#include "../DemoServer/pch/stdafx.h"
#include "TestScene.h"


CTestScene::CTestScene()
	: m_Player{ SizeU(0, 0) }
{
}

CTestScene::~CTestScene()
{
}

bool CTestScene::OnCreate(wstring && tag)
{
	uniform_int_distribution<> pos_random{ 0,11 };
	default_random_engine reng(random_device{}());

	for (int i = 0; i < 12; ++i)
		for (int j = 0; j < 12; ++j)
		{
			if (m_Map.m_ppMap[i][j] == 1) {
				auto block = make_unique<CBlock>(SizeU(i, j));
				m_lstBlock.push_back(move(block));
			}
		}
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

	m_Player.Update(fTimeElapsed);
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