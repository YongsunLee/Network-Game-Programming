#include "stdafx.h"
#include "Framework/IndRes/IndRes.h"
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

void CItem::Draw(ID2D1HwndRenderTarget * RenderTarget)
{
	RenderTarget->DrawBitmap(
		m_bmpImage.Get()
		, m_rcSize + m_ptPoisition
	);
}

void CItem::RegisterImage(CIndRes * indres, ID2D1HwndRenderTarget * RenderTarget, path filename)
{
	LoadImageFromFile(
		indres->wicFactory()
		, RenderTarget
		, filename.c_str()
		, &m_bmpImage
	);

}

void CItem::RegisterImage(const ComPtr<ID2D1Bitmap1>& bmp)
{
	m_bmpImage = bmp;
}

void CItem::RegisterImage(ComPtr<ID2D1Bitmap1>&& bmp) noexcept
{
	m_bmpImage = move(bmp);
}

/////////////////////////////////////////////////////////////////////////

CBomb::CBomb(D2D_SIZE_U coord) : CItem(coord)
{
	m_rcBoomSize = m_rcSize;
	m_rcBoomSize.bottom *= 5;
	m_rcBoomSize.top *= 5;
	m_rcBoomSize.left *= 5;
	m_rcBoomSize.right *= 5;
}

CBomb::~CBomb()
{
}

void CBomb::Update(float fTimeElapsed)
{
	m_fBombTimer -= fTimeElapsed;

	if (m_fBombTimer <= 0) {
		m_bBoom = true;
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

void CBomb::RegisterBoomImage(CIndRes * indres, ID2D1HwndRenderTarget * RenderTarget, path filename)
{
	LoadImageFromFile(
		indres->wicFactory()
		, RenderTarget
		, filename.c_str()
		, &m_bmpBoomImage
	);
}

void CBomb::RegisterBoomImage(const ComPtr<ID2D1Bitmap1>& bmp)
{
	m_bmpBoomImage = bmp;
}



void CBomb::RegisterBoomImage(ComPtr<ID2D1Bitmap1>&& bmp) noexcept
{
	m_bmpBoomImage = move(bmp);
}

void CBomb::Draw(ID2D1HwndRenderTarget * RenderTarget)
{
	if (m_bBoom) {
		RenderTarget->DrawBitmap(
			m_bmpBoomImage.Get()
			, m_rcBoomSize + m_ptPoisition
		);
	}
	RenderTarget->DrawBitmap(
		m_bmpImage.Get()
		, m_rcSize + m_ptPoisition
	);
}
