#include "stdafx.h"
#include "AIViewResultsWnd.h"


CAIViewResultsWnd::CAIViewResultsWnd()
{
    m_nScore = 0;
    m_strUseTime = _T("00:00:00");

    m_pBtnReturn = nullptr;
    m_pBtnLeave = nullptr;
    m_pCtrlPaint50 = nullptr;
    m_pCtrlPaint100 = nullptr;
    m_pCtrlIcon = nullptr;
    m_pLabelPercentage = nullptr;
    m_pLabelTimeData = nullptr;
    m_pLabelEncourage = nullptr;
    m_pLayLeftBtnPanel = nullptr;
    m_pLayRightBtnPanel = nullptr;
}


CAIViewResultsWnd::~CAIViewResultsWnd()
{
}

HWND CAIViewResultsWnd::CreateWnd(HWND hParent)
{
	SetLayerWindow(true);
	ModifyWndStyle(GWL_EXSTYLE, NULL, WS_EX_TOOLWINDOW);

	HWND hWnd = this->Create(hParent, _T("AIViewResultsWnd"));
	if (!hWnd)
		return hWnd;

	RECT rtParent = { 0 };

	if (::GetWindowRect(hParent, &rtParent))
	{
		this->SetWindowPos(NULL,
			rtParent.left,
			rtParent.top,
			rtParent.right - rtParent.left,
			rtParent.bottom - rtParent.top,
			SWP_NOZORDER);
	}

	return hWnd;
}

void CAIViewResultsWnd::OnCreate()
{
    m_pBtnReturn = dynamic_cast<CButtonUI*> (FindControl(_T("btn_view_results_return")));
    m_pBtnLeave = dynamic_cast<CButtonUI*> (FindControl(_T("btn_view_results_leave")));
    m_pCtrlPaint50 = dynamic_cast<CControlUI*> (FindControl(_T("ctrl_panel_paint_50")));
    m_pCtrlPaint100 = dynamic_cast<CControlUI*> (FindControl(_T("ctrl_panel_paint_100")));
    m_pCtrlIcon = dynamic_cast<CControlUI*> (FindControl(_T("ctrl_icon")));
    m_pLabelPercentage = dynamic_cast<CAutoSizeButtonUI*> (FindControl(_T("percentage_label")));
    m_pLabelTimeData = dynamic_cast<CAutoSizeButtonUI*> (FindControl(_T("timedata_label")));
    m_pLabelEncourage = dynamic_cast<CAutoSizeButtonUI*> (FindControl(_T("encourage_label")));
    m_pLayLeftBtnPanel = dynamic_cast<CLayoutUI*> (FindControl(_T("AILeftBtnPanel")));
    m_pLayRightBtnPanel = dynamic_cast<CLayoutUI*> (FindControl(_T("AIRightBtnPanel")));

    UpdateViewData();

	__super::OnCreate();
}

void CAIViewResultsWnd::OnClose()
{

}

bool CAIViewResultsWnd::OnEventReturn(TNotifyUI* pTNotify)
{
    return true;
}

bool CAIViewResultsWnd::OnEventLeave(TNotifyUI* pTNotify)
{
    CloseWindow();
    return true;
}

bool CAIViewResultsWnd::OnEventNo(TNotifyUI* pTNotify)
{
    return true;
}

bool CAIViewResultsWnd::OnEventGo(TNotifyUI* pTNotify)
{
    return true;
}

void CAIViewResultsWnd::SetPercentage(int nScore, int nUseTime)
{
    m_strUseTime = SecondsToTimeString(nUseTime);
    m_nScore = nScore;
}

void CAIViewResultsWnd::UpdateViewData()
{
    if (m_nScore >= 100)
        ShowCtrlStatusOfHundred();
    else if (m_nScore > 0)
        ShowCtrlStatusOfOther();
    else
        ShowCtrlStatusOfZero();

    if (m_pLabelPercentage)
    {
        TCHAR szValue[MAX_PATH];
        _stprintf_s(szValue, _T("%d%%"), m_nScore);
        m_pLabelPercentage->SetText(szValue);
    }

    if (m_pLabelTimeData)
        m_pLabelTimeData->SetText(m_strUseTime.c_str());

    GetRoot()->Resize();
}
void CAIViewResultsWnd::ShowCtrlStatusOfZero()
{
    if (m_pBtnReturn)
        m_pBtnReturn->SetVisible(true);

    if (m_pBtnLeave)
        m_pBtnLeave->SetVisible(true);

    if (m_pCtrlPaint50)
        m_pCtrlPaint50->SetVisible(false);

    if (m_pCtrlPaint100)
        m_pCtrlPaint100->SetVisible(false);

    if (m_pLayLeftBtnPanel)
        m_pLayLeftBtnPanel->SetVisible(false);

    if (m_pLayRightBtnPanel)
        m_pLayRightBtnPanel->SetVisible(false);

    //修改
    if (m_pLabelEncourage)
        m_pLabelEncourage->SetText(I18NSTR(_T("#StrNotGood")));

    if (m_pCtrlIcon)
        m_pCtrlIcon->SetAttribute(_T("bk.image"), _T("#view_results_icon_0"));
}

void CAIViewResultsWnd::ShowCtrlStatusOfOther()
{
    if (m_pBtnReturn)
        m_pBtnReturn->SetVisible(true);

    if (m_pBtnLeave)
        m_pBtnLeave->SetVisible(true);

    if (m_pCtrlPaint50)
        m_pCtrlPaint50->SetVisible(true);

    if (m_pCtrlPaint100)
        m_pCtrlPaint100->SetVisible(false);

    if (m_pLayLeftBtnPanel)
        m_pLayLeftBtnPanel->SetVisible(false);

    if (m_pLayRightBtnPanel)
        m_pLayRightBtnPanel->SetVisible(false);

    //修改
    if (m_pLabelEncourage)
        m_pLabelEncourage->SetText(I18NSTR(_T("#StrComplete")));

    if (m_pCtrlIcon)
        m_pCtrlIcon->SetAttribute(_T("bk.image"), _T("#view_results_icon_50"));
}

void CAIViewResultsWnd::ShowCtrlStatusOfHundred()
{
    if (m_pBtnReturn)
        m_pBtnReturn->SetVisible(false);

    if (m_pBtnLeave)
        m_pBtnLeave->SetVisible(false);

    if (m_pCtrlPaint50)
        m_pCtrlPaint50->SetVisible(false);

    if (m_pCtrlPaint100)
        m_pCtrlPaint100->SetVisible(true);

    if (m_pLayLeftBtnPanel)
        m_pLayLeftBtnPanel->SetVisible(true);

    if (m_pLayRightBtnPanel)
        m_pLayRightBtnPanel->SetVisible(true);

    //修改
    if (m_pLabelEncourage)
        m_pLabelEncourage->SetText(I18NSTR(_T("#StrComplete")));

    if (m_pCtrlIcon)
        m_pCtrlIcon->SetAttribute(_T("bk.image"), _T("#view_results_icon_100"));
}

std::wstring CAIViewResultsWnd::SecondsToTimeString(int nSeconds)
{
    int nHour = nSeconds / 3600;
    int nMin = nSeconds % 3600 / 60;
    int nSec = nSeconds % 60; //不足60的就是秒，够60就是分

    TCHAR szTime[MAX_PATH];
    _stprintf_s(szTime, _T("%02d:%02d:%02d"), nHour, nMin, nSec);

    return std::wstring(szTime);
}
