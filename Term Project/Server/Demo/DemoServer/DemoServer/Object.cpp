#include "../DemoServer/pch/stdafx.h"
#include "Object.h"

CObject::CObject(D2D_POINT_2F pt, D2D_RECT_F rc)
	: m_ptPoisition{ pt }
	, m_rcSize{ rc }
{ 
}

CObject::~CObject()
{
}

bool CObject::Colided(D2D_RECT_F rc)
{
	D2D_RECT_F	colideRect = m_rcSize;
	colideRect = colideRect + m_ptPoisition;
	if (rc.right < colideRect.left) return false;
	if (rc.left > colideRect.right) return false;
	if (rc.bottom < colideRect.top) return false;
	if (rc.top > colideRect.bottom) return false;
	return true;
}
