#include "stdafx.h"
#include "AIViewResultsWnd.h"


CAIViewResultsWnd::CAIViewResultsWnd()
{
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
    CLayoutUI* pLayZero = dynamic_cast<CLayoutUI*>(FindControl(_T("AIViewResultsLayoutZero")));
    CLayoutUI* pLayHundred = dynamic_cast<CLayoutUI*>(FindControl(_T("AIViewResultsLayoutHundred")));
    CLayoutUI* pLayOther = dynamic_cast<CLayoutUI*>(FindControl(_T("AIViewResultsLayoutOther")));
    if (!pLayZero || !pLayHundred || !pLayOther)
        return;

    if (m_eState == Resultsstate_Zero)
    {
        pLayZero->SetVisible(true);
        pLayHundred->SetVisible(false);
        pLayOther->SetVisible(false);
    }
    else if (m_eState == Resultsstate_Hundred)
    {
        pLayZero->SetVisible(false);
        pLayHundred->SetVisible(true);
        pLayOther->SetVisible(false);
    }
    else
    {
        pLayZero->SetVisible(false);
        pLayHundred->SetVisible(false);
        pLayOther->SetVisible(true);
    }

   // GetRoot()->Resize();
   // GetRoot()->Invalidate();

	__super::OnCreate();
}

void CAIViewResultsWnd::OnClose()
{

}
