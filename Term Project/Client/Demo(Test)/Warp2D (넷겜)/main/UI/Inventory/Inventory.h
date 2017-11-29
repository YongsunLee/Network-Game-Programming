#pragma once

#include "UI/UIBase.h"

class CItem;

class CUIInventory
	: public CUIBase
{
public:
	CUIInventory();
	~CUIInventory();

	void DrawClient(ID2D1HwndRenderTarget* pd2dRenderTarget) override;
	void DrawCaption(ID2D1HwndRenderTarget* pd2dRenderTarget) override;
	
	void BuildObject(CScene* scene) override;
	
	void Update(float fTimeElapsed) override;

	// 바닥에 있던 아이템을 인벤토리에서 받아서 사용할 수 있도록
	// Why? 플레이어가 들고있어야 되는것이 아닌가 Item
	// 인벤토리는 UI이다. 플레이어에 소속 (즉, 플레이어를 받아와야 한다.)
	// 플레이어에 통해서 아이템 같은 데이터를 받아와야 한다.
	void GetItem(unique_ptr<CItem>&& item) { m_lstItem.push_back(std::move(item)); }
	void PutItem() { if (m_lstItem.size() > 0) m_lstItem.pop_back(); }
private:

	ComPtr<ID2D1SolidColorBrush>		m_hbrCaption;
	ComPtr<ID2D1SolidColorBrush>		m_hbrClient;
	ComPtr<ID2D1SolidColorBrush>		m_hbrText;

	ComPtr<ID2D1Bitmap1>				m_bmpFrame;

	D2D_SIZE_F							m_szItemOutlineMargin;
	D2D_SIZE_F							m_szItemBetweenMargin;
	D2D_SIZE_U							m_szItemMatrix;

	D2D_RECT_F							m_rcItem;

	list<unique_ptr<CItem>>				m_lstItem;


	ComPtr<IDWriteTextFormat>			m_dwTextFormat;
};
