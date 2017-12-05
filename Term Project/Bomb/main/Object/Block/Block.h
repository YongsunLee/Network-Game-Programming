#pragma once

#include "Object/Object.h"
class CIndRes;

class CBlock
	: public CObject
{
public:

	CBlock(D2D_SIZE_U pt);
	virtual ~CBlock() override;

	virtual void Update(float fTimeElapsed) override;
	virtual void Draw(ID2D1HwndRenderTarget* RenderTarget) override;

	
	virtual void RegisterImage(CIndRes* indres,
		ID2D1HwndRenderTarget* RenderTarget, path filename);

	virtual void RegisterImage(const ComPtr<ID2D1Bitmap1>& bmp);
	virtual void RegisterImage(ComPtr<ID2D1Bitmap1>&& bmp) noexcept;
	

	const D2D_SIZE_U& GetCoord() const { return m_szCoord; }
	
private:

	ComPtr<ID2D1Bitmap1>		m_bmpImage;
	D2D_SIZE_U					m_szCoord;

	UINT						m_iSprite;

};