#pragma once
class CAIFaceLearnWnd
	: public CAILearnBaseWnd
{
public:
	CAIFaceLearnWnd();
	virtual ~CAIFaceLearnWnd();

	HWND			CreateWnd(HWND hParent);

	void			SetFaceQuestion(wstring strFaceQuestion);

protected:
	//override super
	virtual void		OnCreate();
	virtual void		OnClose();

private:
	wstring				m_strFaceQuestion = _T("");

	CButtonUI* m_pBtnFaceCommonTitle = nullptr;
	CButtonUI* m_pBtnFaceTitleTop = nullptr;
	CButtonUI* m_pBtnFaceTitleBottom = nullptr;
};

