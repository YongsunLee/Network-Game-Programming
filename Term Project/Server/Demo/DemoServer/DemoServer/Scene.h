#pragma once

class CScene
{
public:
	CScene();
	virtual ~CScene();

	virtual bool OnCreate(wstring&& tag);
	
	virtual void Update(float fTimeElapsed) = 0;

	bool FindByTag(const wstring& Tag) const { return Tag == m_strTag; }

	const wstring& Tag() const { return m_strTag; }

protected:
	HWND					m_hWnd			{ NULL }	;
	wstring					m_strTag					;
};
