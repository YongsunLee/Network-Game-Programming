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
	virtual void Draw(ID2D1HwndRenderTarget* RenderTarget) override;

	virtual void RegisterImage(CIndRes* indres,
		ID2D1HwndRenderTarget* RenderTarget, path filename);

	virtual void RegisterImage(const ComPtr<ID2D1Bitmap1>& bmp);
	virtual void RegisterImage(ComPtr<ID2D1Bitmap1>&& bmp) noexcept;

	ID2D1Bitmap1* GetBitmap() const { return m_bmpImage.Get(); }

	bool IsCollision(D2D_SIZE_U sz) const { return sz == m_szCoord; }
	const D2D_SIZE_U& GetCoord() const { return m_szCoord; }

	void ResetPosition() { SetPosition(Point2F()); }

protected:

	ComPtr<ID2D1Bitmap1>		m_bmpImage;
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
	void SetBoom() { m_bBoom = true; }

	bool GetBoom() { return m_bBoom; }

protected:
	float						m_fBombTimer	{ 3 };
	bool						m_bBoom			{false};
};