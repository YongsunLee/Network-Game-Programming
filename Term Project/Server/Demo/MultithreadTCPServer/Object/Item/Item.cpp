#include "stdafx.h"
#include "Item.h"

CItem::CItem(D2D_SIZE_U sz)
	: CObject(Point2F(sz.width * g_fTileWidth, sz.height * g_fTileHeight), g_rcItemRect)
	, m_szCoord{ sz }
{
}

CItem::~CItem()
{
}

void CItem::Update(float fTimeElapsed)
{

}

/////////////////////////////////////////////////////////////////////////

CBomb::CBomb(D2D_SIZE_U coord) : CItem(coord)
{
}

CBomb::~CBomb()
{
}

void CBomb::Update(float fTimeElapsed)
{
	m_fBombTimer -= fTimeElapsed;

	if(!m_bBoom)
	if (m_fBombTimer <= 0) {
		SetBoom();
	}
}

bool CBomb::ColideBoom(D2D_SIZE_U coord)
{
	for (int i = -2; i < 3; ++i) {
		D2D_SIZE_U retval = GetCoord();
		retval.width += i;
		if (retval == coord) {
			return true;
		}
	}
	for (int i = -2; i < 3; ++i) {
		D2D_SIZE_U retval = GetCoord();
		retval.height += i;
		if (retval == coord) {
			return true;
		}
	}
	return false;
}

bool CBomb::ColideBoom(D2D_RECT_F rect)
{
	for (int i = -2; i < 3; ++i) {
		D2D_SIZE_U retval = GetCoord();
		retval.width += i;
		
		D2D_POINT_2F pos = Point2F(retval.width * g_fTileWidth, retval.height * g_fTileHeight);

		if(Colided(m_rcSize+ pos, rect)) {
			return true;
		}
	}
	for (int i = -2; i < 3; ++i) {
		D2D_SIZE_U retval = GetCoord();
		retval.height += i;

		D2D_POINT_2F pos = Point2F(retval.width * g_fTileWidth, retval.height * g_fTileHeight);
		if (Colided(m_rcSize+pos, rect)) {
			return true;
		}
	}
	return false;
}
