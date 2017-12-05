#pragma once

#include "Object/Object.h"
class CIndRes;

class CPlayer
	: public CObject
{
public:
	enum Dir { left = 1, top = 3, right = 2, bottom = 0 };

	CPlayer(D2D_SIZE_U pt);
	virtual ~CPlayer() override;

	virtual void Update(float fTimeElapsed) override;
	virtual void Draw(ID2D1HwndRenderTarget* RenderTarget) override;

	virtual void RegisterImage(CIndRes* indres,
		ID2D1HwndRenderTarget* RenderTarget, path filename, D2D_SIZE_U szSprite);

	virtual void RegisterImage(const ComPtr<ID2D1Bitmap1>& bmp, D2D_SIZE_U szSprite);
	virtual void RegisterImage(ComPtr<ID2D1Bitmap1>&& bmp, D2D_SIZE_U szSprite) noexcept;

	void SetMoveX(float move) {
		if (IsActive)m_Move.x = move;
	}
	void SetMoveY(float move) { m_Move.y = move; }
	void SetMove(D2D_POINT_2F move) { m_Move = move;}
	void Move(float fTimeElapsed);
	void Move(D2D_POINT_2F move,float fTimeElapsed);
	void SetHorizon(float x) { m_Move.x = x; }
	void SetVerticla(float y) { m_Move.y = y; }
	void CloseCoord();
	void CheckInput();


	D2D_POINT_2F GetMove() { return m_Move; }
	float GetMoveX() {return m_Move.x; }
	float GetMoveY() {return m_Move.y; }

	const D2D_SIZE_U& GetCoord() const { return m_szCoord; }

	void SetActive(bool b) { IsActive = b; }
private:

	ComPtr<ID2D1Bitmap1>		m_bmpImage;
	D2D_SIZE_U					m_szCoord;

	D2D_POINT_2F				m_Position;
	D2D_POINT_2F				m_Move;

	Dir							m_Direction;
	D2D_SIZE_U					m_szSprite;
	UINT						m_iSprite;

	float						m_fBombCnt;
	float						m_fTick;
	bool						IsActive{true};
};
