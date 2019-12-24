#include "stdafx.h"
#include "Singleton.h"
#include "AIActivityWnd.h"
#include "AIViewResultsWnd.h"
#include "AICabinWnd.h"
#include "MainWnd.h"
#include "Application.h"

CAICabinWnd::CAICabinWnd()
{
	ModifyWndStyle(GWL_STYLE, WS_VISIBLE, 0);
}


CAICabinWnd::~CAICabinWnd()
{
}

HWND CAICabinWnd::CreateWnd()
{
	//SetLayerWindow(true);

	return Create(HWND_DESKTOP, _T("AICabinWnd"));
}

void CAICabinWnd::OnCreate()
{
	SetWindowClassName(_T("AICabinWnd"));
	__super::OnCreate();
}

void CAICabinWnd::OnClose()
{
	PostMessage(CApplication::GetInstance()->GetMainHwnd(), WM_QUIT, 0, 0);
}

LRESULT CAICabinWnd::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	if (message == WM_KEYDOWN)
	{
		switch (wParam)
		{
		case VK_ESCAPE:
		{
			break;
		}
		case VK_F5:
		{
			CAIActivityWnd* pAIActivityWnd = new CAIActivityWnd;
			if (pAIActivityWnd)
			{
				pAIActivityWnd->CreateWnd(GetHWND());
				pAIActivityWnd->ShowWindow();
			}
			break;
		}
        case VK_F6:
        {
            CAIViewResultsWnd* pAIViewResultsWnd = new CAIViewResultsWnd;
            if (pAIViewResultsWnd)
            {
                pAIViewResultsWnd->SetViewResultsState(Resultsstate_Zero);
                pAIViewResultsWnd->CreateWnd(GetHWND());
                pAIViewResultsWnd->ShowWindow();
            }
            break;
        }
        case VK_F7:
        {
            CAIViewResultsWnd* pAIViewResultsWnd = new CAIViewResultsWnd;
            if (pAIViewResultsWnd)
            {
                pAIViewResultsWnd->SetViewResultsState(Resultsstate_Other);
                pAIViewResultsWnd->CreateWnd(GetHWND());
                pAIViewResultsWnd->ShowWindow();
            }
            break;
        }
        case VK_F8:
        {
            CAIViewResultsWnd* pAIViewResultsWnd = new CAIViewResultsWnd;
            if (pAIViewResultsWnd)
            {
                pAIViewResultsWnd->SetViewResultsState(Resultsstate_Hundred);
                pAIViewResultsWnd->CreateWnd(GetHWND());
                pAIViewResultsWnd->ShowWindow();
            }
            break;
        }
		default:
			break;
		}
	}

	return __super::WindowProc(message, wParam, lParam);
}

void CAICabinWnd::SwitchLayout()
{

}
