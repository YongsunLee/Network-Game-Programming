#include "stdafx.h"
#include "Block.h"



CBlock::CBlock(D2D_SIZE_U sz)
	: CObject(Point2F(sz.width * g_fTileWidth, sz.height * g_fTileHeight), g_rcItemRect)
	, m_szCoord{ sz }
{
}

CBlock::~CBlock()
{
}

void CBlock::Update(float fTimeElapsed)
{
}