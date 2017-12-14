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
	CBomb(D2D_SIZE_U coord = SizeU(0, 0));
	virtual ~CBomb() override;
	virtual void Update(float fTimeElapsed) override;
	
	bool ColideBoom(D2D_SIZE_U coord);
	bool ColideBoom(D2D_RECT_F rect);
	void SetBoom() { m_bBoom = true; }
	void SetBoomState(bool state) { m_bBoom = state; }
	bool GetBoom() { return m_bBoom; }
	ID2D1Bitmap1* GetBoomBitmap() const { return m_bmpBoomImage.Get(); }
	void RegisterBoomImage(CIndRes * indres, ID2D1HwndRenderTarget * RenderTarget, path filename);
	void RegisterBoomImage(const ComPtr<ID2D1Bitmap1>& bmp);
	void RegisterBoomImage(ComPtr<ID2D1Bitmap1>&& bmp) noexcept;
	virtual void Draw(ID2D1HwndRenderTarget* RenderTarget) override;
protected:
	ComPtr<ID2D1Bitmap1>		m_bmpBoomImage;
	float						m_fBombTimer	{ 3 };
	bool						m_bBoom			{false};
	D2D_RECT_F					m_rcBoomSize;
};