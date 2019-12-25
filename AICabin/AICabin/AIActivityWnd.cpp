#include "stdafx.h"
#include "Singleton.h"
#include "AILearnBaseWnd.h"
#include "AIActivityWnd.h"
#include "AISpeakLearnWnd.h"
#include "AICabinWnd.h"
#include "Application.h"

CAIActivityWnd::CAIActivityWnd()
{
}


CAIActivityWnd::~CAIActivityWnd()
{
}

HWND CAIActivityWnd::CreateWnd(HWND hParent)
{
	SetLayerWindow(true);
	ModifyWndStyle(GWL_EXSTYLE, NULL, WS_EX_TOOLWINDOW);

	HWND hWnd = this->Create(hParent, _T("AIActivityWnd"));
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

void CAIActivityWnd::OnCreate()
{
	__super::OnCreate();

    CLayoutUI *pLayActivityTwo = dynamic_cast<CLayoutUI*>(FindControl(_T("activity_two_layout")));
    if (pLayActivityTwo)
        pLayActivityTwo->OnEvent += MakeDelegate(this, &CAIActivityWnd::OnLayoutListenReadEvent);

    /*CButtonUI *pBtnReturnLeft = dynamic_cast<CButtonUI*>(FindControl(_T("btn_left_panel_return")));
    if (pBtnReturnLeft)
        pBtnReturnLeft->SetEnabled(false);

    CButtonUI *pBtnReturnRight = dynamic_cast<CButtonUI*>(FindControl(_T("btn_right_panel_return")));
    if (pBtnReturnRight)
        pBtnReturnRight->SetEnabled(false);*/

	if (m_pAILeftBtnPanel
		&& m_pAILeftBtnPanelUnExpend)
	{
		m_pAILeftBtnPanel->SetVisible(false);
		m_pAILeftBtnPanelUnExpend->SetVisible(true);
	}
	
    CApplication::GetInstance()->m_pAIActivityWnd = this;
}

void CAIActivityWnd::OnClose()
{
    CApplication::GetInstance()->m_pAIActivityWnd = nullptr;
}

bool CAIActivityWnd::OnEventReturn(TNotifyUI* pTNotify)
{
    //返回到上一界面
    //CloseWindow();

    return true;
}

bool CAIActivityWnd::OnEventLeave(TNotifyUI* pTNotify)
{
    //关闭活动列表窗口
    CloseWindow();

    //跳到结束界面
    if (CApplication::GetInstance()->m_pAICabinWnd != nullptr)
        (CApplication::GetInstance()->m_pAICabinWnd)->ShowEndLayout();

    return true;
}

bool CAIActivityWnd::OnLayoutBodyPartsEvent(TEventUI& evt)
{
    return true;
}

bool CAIActivityWnd::OnLayoutListenReadEvent(TEventUI& evt)
{
    if (evt.nType != UIEVENT_BUTTONDOWN)
        return true;

    CAISpeakLearnWnd* pAISpeakLearnWnd = new CAISpeakLearnWnd;
    if (pAISpeakLearnWnd)
    {
        pAISpeakLearnWnd->CreateWnd(GetHWND());
        pAISpeakLearnWnd->ShowWindow();
    }

    return true;
}

bool CAIActivityWnd::OnLayoutVrAiEvent(TEventUI& evt)
{
    return true;
}
