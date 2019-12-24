#include "stdafx.h"
#include "AILearnBaseWnd.h"
#include "AIActivityWnd.h"
#include "AISpeakLearnWnd.h"


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


	if (m_pAILeftBtnPanel
		&& m_pAILeftBtnPanelUnExpend)
	{
		m_pAILeftBtnPanel->SetVisible(false);
		m_pAILeftBtnPanelUnExpend->SetVisible(true);
	}
	
}

void CAIActivityWnd::OnClose()
{

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
