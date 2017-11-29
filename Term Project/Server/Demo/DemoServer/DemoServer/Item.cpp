#include "../DemoServer/pch/stdafx.h"
#include "Item.h"

CItem::CItem(D2D_SIZE_U sz)
	: CObject(Point2F(sz.width * g_fTileWidth, sz.height * g_fTileHeight), g_rcItemRect)
	, m_szCoord { sz }
{
}

CItem::~CItem()
{
}

void CItem::Update(float fTimeElapsed)
{

}