#include "stdafx.h"
#include "Singleton.h"
#include "AILearnBaseWnd.h"
#include "AIViewResultsWnd.h"
#include "Application.h"
#include "AIFaceLearnWnd.h"
#include "AISpeakLearnWnd.h"
#include "AIActivityWnd.h"
#include "AICabinWnd.h"

CAIViewResultsWnd::CAIViewResultsWnd()
{
    m_nScore = 0;
    m_strUseTime = _T("00:00:00");

    m_pCtrlPaint50 = nullptr;
    m_pCtrlPaint100 = nullptr;
    m_pCtrlIcon = nullptr;
    m_pLabelPercentage = nullptr;
    m_pLabelTimeData = nullptr;
    m_pLabelEncourage = nullptr;
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
	__super::OnCreate();
    m_pCtrlPaint50 = dynamic_cast<CControlUI*> (FindControl(_T("ctrl_panel_paint_50")));
    m_pCtrlPaint100 = dynamic_cast<CControlUI*> (FindControl(_T("ctrl_panel_paint_100")));
    m_pCtrlIcon = dynamic_cast<CControlUI*> (FindControl(_T("ctrl_icon")));
    m_pLabelPercentage = dynamic_cast<CAutoSizeButtonUI*> (FindControl(_T("percentage_label")));
    m_pLabelTimeData = dynamic_cast<CAutoSizeButtonUI*> (FindControl(_T("timedata_label")));
    m_pLabelEncourage = dynamic_cast<CAutoSizeButtonUI*> (FindControl(_T("encourage_label")));

    UpdateViewData();	

	if (m_pAILeftBtnPanel
		&& m_pAILeftBtnPanelUnExpend)
	{
		m_pAILeftBtnPanel->SetVisible(true);
		m_pAILeftBtnPanelUnExpend->SetVisible(false);
	}
}

void CAIViewResultsWnd::OnClose()
{

}

bool CAIViewResultsWnd::OnEventReturn(TNotifyUI* pTNotify)
{
    //关闭查看成绩窗口
    CloseWindow();

    //关闭人脸表情窗口
    if (CApplication::GetInstance()->m_pAIFaceLearnWnd != nullptr)
        (CApplication::GetInstance()->m_pAIFaceLearnWnd)->CloseWindow();

    //关闭学习卡片窗口
    if (CApplication::GetInstance()->m_pAISpeakLearnWnd != nullptr)
        (CApplication::GetInstance()->m_pAISpeakLearnWnd)->CloseWindow();

    //跳到活动界面
    if (CApplication::GetInstance()->m_pAIActivityWnd != nullptr)
        (CApplication::GetInstance()->m_pAIActivityWnd)->ShowWindowData();
    return true;
}

bool CAIViewResultsWnd::OnEventLeave(TNotifyUI* pTNotify)
{
    //关闭查看成绩窗口
    CloseWindow();

    //关闭人脸表情窗口
    if (CApplication::GetInstance()->m_pAIFaceLearnWnd != nullptr)
        (CApplication::GetInstance()->m_pAIFaceLearnWnd)->CloseWindow();

    //关闭学习卡片窗口
    if (CApplication::GetInstance()->m_pAISpeakLearnWnd != nullptr)
        (CApplication::GetInstance()->m_pAISpeakLearnWnd)->CloseWindow();

    //关闭活动列表窗口
    if (CApplication::GetInstance()->m_pAIActivityWnd != nullptr)
        (CApplication::GetInstance()->m_pAIActivityWnd)->CloseWindow();

    //跳到结束界面
    if (CApplication::GetInstance()->m_pAICabinWnd != nullptr)
        (CApplication::GetInstance()->m_pAICabinWnd)->ShowEndLayout();

    return true;
}

bool CAIViewResultsWnd::OnEventNo(TNotifyUI* pTNotify)
{
    //关闭查看成绩窗口
    CloseWindow();

    //关闭人脸表情窗口
    if (CApplication::GetInstance()->m_pAIFaceLearnWnd != nullptr)
        (CApplication::GetInstance()->m_pAIFaceLearnWnd)->CloseWindow();

    //关闭学习卡片窗口
    if (CApplication::GetInstance()->m_pAISpeakLearnWnd != nullptr)
        (CApplication::GetInstance()->m_pAISpeakLearnWnd)->CloseWindow();

    //跳到活动界面
    if (CApplication::GetInstance()->m_pAIActivityWnd != nullptr)
        (CApplication::GetInstance()->m_pAIActivityWnd)->ShowWindowData();

    return true;
}

bool CAIViewResultsWnd::OnEventGo(TNotifyUI* pTNotify)
{
    //关闭查看成绩窗口
    CloseWindow();

    //关闭人脸表情窗口
    if (CApplication::GetInstance()->m_pAIFaceLearnWnd != nullptr)
        (CApplication::GetInstance()->m_pAIFaceLearnWnd)->CloseWindow();

    //跳到卡片开始学习界面
    if (CApplication::GetInstance()->m_pAISpeakLearnWnd != nullptr)
        (CApplication::GetInstance()->m_pAISpeakLearnWnd)->StartNewRecord();

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
    if (m_pCtrlPaint50)
        m_pCtrlPaint50->SetVisible(false);

    if (m_pCtrlPaint100)
        m_pCtrlPaint100->SetVisible(false);

    //修改
    if (m_pLabelEncourage)
        m_pLabelEncourage->SetText(I18NSTR(_T("#StrNotGood")));

    if (m_pCtrlIcon)
        m_pCtrlIcon->SetAttribute(_T("bk.image"), _T("#view_results_icon_0"));
}

void CAIViewResultsWnd::ShowCtrlStatusOfOther()
{
    if (m_pCtrlPaint50)
        m_pCtrlPaint50->SetVisible(true);

    if (m_pCtrlPaint100)
        m_pCtrlPaint100->SetVisible(false);

    //修改
    if (m_pLabelEncourage)
        m_pLabelEncourage->SetText(I18NSTR(_T("#StrComplete")));

    if (m_pCtrlIcon)
        m_pCtrlIcon->SetAttribute(_T("bk.image"), _T("#view_results_icon_50"));
}

void CAIViewResultsWnd::ShowCtrlStatusOfHundred()
{
    if (m_pCtrlPaint50)
        m_pCtrlPaint50->SetVisible(false);

    if (m_pCtrlPaint100)
        m_pCtrlPaint100->SetVisible(true);

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
