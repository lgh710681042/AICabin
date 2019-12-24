#pragma once

class CAILearnBaseWnd
	: public CWindowUI
{
public:
	CAILearnBaseWnd();
	virtual ~CAILearnBaseWnd();

	UIBEGIN_MSG_MAP
		UI_EVENT_ID_HANDLER(UINOTIFY_CLICK, _T("btn_left_panel_arraw"), OnBtnLeftPanelArraw);
		UI_EVENT_ID_HANDLER(UINOTIFY_CLICK, _T("btn_left_panel_arraw_un"), OnBtnLeftPanelArrawUn);
		UI_EVENT_ID_HANDLER(UINOTIFY_CLICK, _T("btn_right_panel_arraw"), OnBtnRightPanelArraw);
		UI_EVENT_ID_HANDLER(UINOTIFY_CLICK, _T("btn_right_panel_arraw_un"), OnBtnRightPanelArrawUn);
	UIEND_MSG_MAP

protected:
	//override super
	virtual void		OnCreate();

private:
	bool                OnBtnLeftPanelArraw(TNotifyUI* pTNotify);
	bool                OnBtnLeftPanelArrawUn(TNotifyUI* pTNotify);
	bool                OnBtnRightPanelArraw(TNotifyUI* pTNotify);
	bool                OnBtnRightPanelArrawUn(TNotifyUI* pTNotify);

private:
	//◊Û”“¡Ω±ﬂ∞¥≈•√Ê∞Â
	CLayoutUI*			m_pAILeftBtnPanel = nullptr;
	CLayoutUI*			m_pAILeftBtnPanelUnExpend = nullptr;
	CLayoutUI*			m_pAIRightBtnPanel = nullptr;
	CLayoutUI*			m_pAIRightBtnPanelUnExpend = nullptr;
};

