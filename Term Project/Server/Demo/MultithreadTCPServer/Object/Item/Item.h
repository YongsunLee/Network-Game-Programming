#pragma once
#include "Object/Object.h"



class CIndRes;

class CItem 
	: public CObject
{
public:
	CItem(D2D_SIZE_U coord);
	virtual ~CItem() override;

	virtual void Update(float fTimeElapsed) override;

	bool IsCollision(D2D_SIZE_U sz) const { return sz == m_szCoord; }
	const D2D_SIZE_U& GetCoord() const { return m_szCoord; }

	void ResetPosition() { SetPosition(Point2F()); }

protected:

	D2D_SIZE_U					m_szCoord;
};

class CBomb
	:public CItem
{
public :
	CBomb(D2D_SIZE_U coord);
	virtual ~CBomb() override;
	virtual void Update(float fTimeElapsed) override;
	
	bool ColideBoom(D2D_SIZE_U coord);
	bool ColideBoom(D2D_RECT_F rect);
	void SetBoom() { 
		if (!m_bBoom) {

		m_fBombTimer = 1.f;
		m_bBoom = true; 
		}
	}

	bool GetBoom() { return m_bBoom; }
	float GetTime() { return m_fBombTimer; }

protected:
	float						m_fBombTimer	{ 3 };
	bool						m_bBoom			{false};
};