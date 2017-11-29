#pragma once
#include "../DemoServer/Object.h"
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
