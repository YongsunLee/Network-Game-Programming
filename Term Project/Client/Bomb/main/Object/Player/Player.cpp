#include "stdafx.h"
#include "Framework/IndRes/IndRes.h"
#include "Player.h"

CPlayer::CPlayer(D2D_SIZE_U sz)
	: CObject(Point2F(sz.width * g_fTileWidth, sz.height * g_fTileHeight), g_rcPlayermRect)
	, m_szCoord{ sz }, m_Direction{ Dir::bottom }
{
}

CPlayer::~CPlayer()
{
}

void CPlayer::Update(float fTimeElapsed)
{
	if (IsActive)
	{

		m_fTick += (fTimeElapsed * 5.f);
		if (m_fTick > 4.f)
			m_fTick -= 4.f;

		//CheckInput();
		//Move(fTimeElapsed);
	}
	//CloseCoord();
}

void CPlayer::Draw(ID2D1HwndRenderTarget * RenderTarget)
{
	auto szSprite = m_bmpImage->GetSize();
	szSprite.width /= m_szSprite.width;
	szSprite.height /= m_szSprite.height;

	auto szSrc = SizeF(szSprite.width * static_cast<UINT>(m_fTick), szSprite.height * m_Direction);
	if (win) {
		
		RenderTarget->DrawBitmap(
			m_bmpWinImage.Get()
			, g_rcWinRect + m_ptPoisition
		);

	}

	RenderTarget->DrawBitmap(
		m_bmpImage.Get()
		, m_rcSize + m_ptPoisition
		, 1.f, D2D1_BITMAP_INTERPOLATION_MODE_LINEAR
		, RectF(szSrc.width, szSrc.height, szSrc.width + szSprite.width, szSrc.height + szSprite.height)
	);
	if (!IsActive)
	{
		ComPtr<ID2D1SolidColorBrush> hbr;
		RenderTarget->CreateSolidColorBrush(ColorF{ ColorF::Black, 0.6f }, &hbr);
		RenderTarget->SetAntialiasMode(D2D1_ANTIALIAS_MODE_ALIASED);

		RenderTarget->FillOpacityMask(
			m_bmpImage.Get()
			, hbr.Get()
			, D2D1_OPACITY_MASK_CONTENT_GRAPHICS
			, m_rcSize + m_ptPoisition
			, RectF(szSrc.width, szSrc.height, szSrc.width + szSprite.width, szSrc.height + szSprite.height)

		);

		RenderTarget->SetAntialiasMode(D2D1_ANTIALIAS_MODE_PER_PRIMITIVE);
	}
}

void CPlayer::RegisterImage(CIndRes * indres, ID2D1HwndRenderTarget * RenderTarget, path filename, D2D_SIZE_U szSprite)
{
	LoadImageFromFile(
		indres->wicFactory()
		, RenderTarget
		, filename.c_str()
		, &m_bmpImage
	);
	m_szSprite = szSprite;
}

void CPlayer::RegisterImage(const ComPtr<ID2D1Bitmap1>& bmp, D2D_SIZE_U szSprite)
{
	m_bmpImage = bmp;
	m_szSprite = szSprite;
}

void CPlayer::RegisterImage(ComPtr<ID2D1Bitmap1>&& bmp, D2D_SIZE_U szSprite) noexcept
{
	m_bmpImage = move(bmp);
	m_szSprite = szSprite;
}

void CPlayer::Move(float fTimeElapsed)
{
	m_ptPoisition = m_ptPoisition + (100 * fTimeElapsed*m_Move);
	if (m_ptPoisition.x < 0) {
		m_ptPoisition.x = 0;
	}
	else if (m_ptPoisition.x > 220) {
		m_ptPoisition.x = 220;
	}
	if (m_ptPoisition.y < 0) {
		m_ptPoisition.y = 0;
	}
	else if (m_ptPoisition.y > 220) {
		m_ptPoisition.y = 220;
	}
}

void CPlayer::Move(D2D_POINT_2F move, float fTimeElapsed)
{
	m_ptPoisition = m_ptPoisition + (100 * fTimeElapsed*move);
	if (m_ptPoisition.x < 0) {
		m_ptPoisition.x = 0;
	}
	else if (m_ptPoisition.x > 220) {
		m_ptPoisition.x = 220;
	}
	if (m_ptPoisition.y < 0) {
		m_ptPoisition.y = 0;
	}
	else if (m_ptPoisition.y > 220) {
		m_ptPoisition.y = 220;
	}
}

void CPlayer::CloseCoord()
{
	D2D_SIZE_U retval;

	retval.width = (int)((m_ptPoisition.x + m_rcSize.right) / g_fTileWidth);
	retval.height = (int)((m_ptPoisition.y + m_rcSize.bottom) / g_fTileHeight);

	m_szCoord = retval;
}

void CPlayer::CheckInput()
{
	//if (m_Move.x != 0 || m_Move.y != 0) {

	if (m_Move.x > 0) {
		m_Direction = right;
	}
	else if (m_Move.x < 0) {
		m_Direction = left;
	}
	else if (m_Move.y > 0) {
		m_Direction = bottom;
	}
	else if (m_Move.y < 0) {
		m_Direction = top;
	}
	//}
}

void CPlayer::RegisterWinImage(CIndRes * indres, ID2D1HwndRenderTarget * RenderTarget, path filename)
{
	LoadImageFromFile(
		indres->wicFactory()
		, RenderTarget
		, filename.c_str()
		, &m_bmpWinImage
	);
}

void CPlayer::RegisterWinImage(const ComPtr<ID2D1Bitmap1>& bmp)
{
	m_bmpWinImage = bmp;
}

void CPlayer::RegisterWinImage(ComPtr<ID2D1Bitmap1>&& bmp) noexcept
{
	m_bmpWinImage = move(bmp);
}
