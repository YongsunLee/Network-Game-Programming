#include "stdafx.h"
#include "Player.h"

CPlayer::CPlayer(D2D_SIZE_U sz)
	: CObject(Point2F(sz.width * g_fTileWidth, sz.height * g_fTileHeight), g_rcPlayermRect)
	, m_szCoord{ sz }
{
}

CPlayer::~CPlayer()
{
}

void CPlayer::Update(float fTimeElapsed)
{
	CheckInput();
	Move(fTimeElapsed);
	CloseCoord();
}

void CPlayer::Move(float fTimeElapsed)
{
	m_ptPoisition = m_ptPoisition + (100 * fTimeElapsed*m_Move);
	if (m_ptPoisition.x < 0) {
		m_ptPoisition.x = 0;
	}
	else if (m_ptPoisition.x >220) {
		m_ptPoisition.x = 220;
	}
	if (m_ptPoisition.y < 0) {
		m_ptPoisition.y = 0;
	}
	else if (m_ptPoisition.y >220) {
		m_ptPoisition.y = 220;
	}
}

void CPlayer::Move(D2D_POINT_2F move, float fTimeElapsed)
{
	m_ptPoisition = m_ptPoisition + (100 * fTimeElapsed*move);
	if (m_ptPoisition.x < 0) {
		m_ptPoisition.x = 0;
	}
	else if (m_ptPoisition.x >220) {
		m_ptPoisition.x = 220;
	}
	if (m_ptPoisition.y < 0) {
		m_ptPoisition.y = 0;
	}
	else if (m_ptPoisition.y >220) {
		m_ptPoisition.y = 220;
	}
}

void CPlayer::CloseCoord()
{
	D2D_SIZE_U retval;

	retval.width = (int)((m_ptPoisition.x + m_rcSize.right )/ g_fTileWidth);
	retval.height = (int)((m_ptPoisition.y+m_rcSize.bottom) / g_fTileHeight);

	m_szCoord = retval;
}

void CPlayer::CheckInput()
{
	if (m_Move.x != 0 || m_Move.y != 0) {

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
	}
}
