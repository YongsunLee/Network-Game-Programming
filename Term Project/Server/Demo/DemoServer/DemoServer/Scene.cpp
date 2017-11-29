#include "../DemoServer/pch/stdafx.h"
#include "Scene.h"

CScene::CScene()
{
}

CScene::~CScene()
{
}

bool CScene::OnCreate(wstring&& tag)
{
	m_strTag = move(tag);

	return true;
}
