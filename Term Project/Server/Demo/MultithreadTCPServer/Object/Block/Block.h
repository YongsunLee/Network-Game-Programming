#pragma once

#include "Object/Object.h"
class CIndRes;

class CBlock
	: public CObject
{
public:

	CBlock() {}
	CBlock(D2D_SIZE_U pt);
	virtual ~CBlock() override;

	virtual void Update(float fTimeElapsed) override;
	

	const D2D_SIZE_U& GetCoord() const { return m_szCoord; }
	
private:

	D2D_SIZE_U					m_szCoord;

	UINT						m_iSprite;

};