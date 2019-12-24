#pragma once
class CAIFaceLearnWnd
	: public CAILearnBaseWnd
{
public:
	CAIFaceLearnWnd();
	virtual ~CAIFaceLearnWnd();

	UIBEGIN_MSG_MAP
		UI_EVENT_ID_HANDLER(UINOTIFY_CLICK, _T("btn_left_panel_arraw"), OnBtnLeftPanelArraw);
		UI_EVENT_ID_HANDLER(UINOTIFY_CLICK, _T("btn_left_panel_arraw_un"), OnBtnLeftPanelArrawUn);
		UI_EVENT_ID_HANDLER(UINOTIFY_CLICK, _T("btn_right_panel_arraw"), OnBtnRightPanelArraw);
		UI_EVENT_ID_HANDLER(UINOTIFY_CLICK, _T("btn_right_panel_arraw_un"), OnBtnRightPanelArrawUn);
	UIEND_MSG_MAP

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

