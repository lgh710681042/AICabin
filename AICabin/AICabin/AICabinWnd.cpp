#include "stdafx.h"
#include "Singleton.h"
#include "AILearnBaseWnd.h"
#include "AIActivityWnd.h"
#include "AIViewResultsWnd.h"
#include "AISpeakLearnWnd.h"
#include "AIFaceLearnWnd.h"
#include "AICabinWnd.h"
#include "MainWnd.h"
#include "Application.h"
#include "SpeechSynthControl.h"

#define SHOW_ACTIVITY_TIMER			1001
#ifdef _DEBUG
#define SHOW_ACTIVITY_INTERVAL		1000
#else
#define SHOW_ACTIVITY_INTERVAL		8000
#endif


#define TIMER_EARTH_ID			1003					// 转动定时器
#define TIMER_EARTH_TIME		50						// 每66ms切换图片

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

void CAICabinWnd::SetFullScreen(bool fullScreen)
{
	if (!fullscreen_)
	{
		saved_window_info_.style = GetWindowLong(GetHWND(), GWL_STYLE);
		saved_window_info_.ex_style = GetWindowLong(GetHWND(), GWL_EXSTYLE);
		GetWindowRect(GetHWND(), &saved_window_info_.window_rect);
	}

	fullscreen_ = fullScreen;

	if (fullscreen_)
	{
		// Set new window style and size.
		SetWindowLong(GetHWND(), GWL_STYLE,
			saved_window_info_.style & ~(WS_CAPTION | WS_THICKFRAME));
		SetWindowLong(GetHWND(), GWL_EXSTYLE,
			saved_window_info_.ex_style & ~(WS_EX_DLGMODALFRAME |
			WS_EX_WINDOWEDGE | WS_EX_CLIENTEDGE | WS_EX_STATICEDGE));

		// On expand, if we're given a window_rect, grow to it, otherwise do
		// not resize.
		MONITORINFO monitor_info;
		monitor_info.cbSize = sizeof(monitor_info);
		GetMonitorInfo(MonitorFromWindow(GetHWND(), MONITOR_DEFAULTTONEAREST),
			&monitor_info);

		SetWindowPos(NULL, monitor_info.rcMonitor.left, monitor_info.rcMonitor.top,
			monitor_info.rcMonitor.right - monitor_info.rcMonitor.left,
			monitor_info.rcMonitor.bottom - monitor_info.rcMonitor.top,
			SWP_NOZORDER | SWP_NOACTIVATE | SWP_FRAMECHANGED);

		SetAttribute(_T("move"), _T("0"));
	}
	else
	{
		// Reset original window style and size.  The multiple window size/moves
		// here are ugly, but if SetWindowPos() doesn't redraw, the taskbar won't be
		// repainted.  Better-looking methods welcome.
		SetWindowLong(GetHWND(), GWL_STYLE, saved_window_info_.style);
		SetWindowLong(GetHWND(), GWL_EXSTYLE, saved_window_info_.ex_style);

		SetWindowPos(NULL, saved_window_info_.window_rect.left, saved_window_info_.window_rect.top,
			saved_window_info_.window_rect.right - saved_window_info_.window_rect.left,
			saved_window_info_.window_rect.bottom - saved_window_info_.window_rect.top,
			SWP_NOZORDER | SWP_NOACTIVATE | SWP_FRAMECHANGED);

		SetAttribute(_T("move"), _T("1"));
	}

	MarkFullScreen(fullScreen);
}

void CAICabinWnd::MarkFullScreen(bool fullscreen)
{
	if (!task_bar_list_) {
		HRESULT hr =
			::CoCreateInstance(CLSID_TaskbarList, NULL, CLSCTX_INPROC_SERVER,
			IID_PPV_ARGS(&task_bar_list_));
		if (SUCCEEDED(hr) && FAILED(task_bar_list_->HrInit()))
			task_bar_list_ = nullptr;
	}

	// As per MSDN marking the window as fullscreen should ensure that the
	// taskbar is moved to the bottom of the Z-order when the fullscreen window
	// is activated. If the window is not fullscreen, the Shell falls back to
	// heuristics to determine how the window should be treated, which means
	// that it could still consider the window as fullscreen. :(
	if (task_bar_list_)
		task_bar_list_->MarkFullscreenWindow(GetHWND(), !!fullscreen);
}

wstring CAICabinWnd::SetTipsStart(wstring& strUserName)
{
	WCHAR szBuf[1024];
	_stprintf_s(szBuf, _countof(szBuf), I18NSTR(_T("#StrTipStart")), strUserName.c_str());

	if (m_pButtonTipsStart)
	{
		m_pButtonTipsStart->SetText(szBuf);
		m_pButtonTipsStart->Invalidate();
	}

	return szBuf;
}

bool CAICabinWnd::OnTimer(TEventUI& event)
{
	if (event.nType == UIEVENT_TIMER)
	{
		if (event.wParam == SHOW_ACTIVITY_TIMER)
		{
			//显示活动列表窗口
			CAIActivityWnd* pAIActivityWnd = new CAIActivityWnd;
			if (pAIActivityWnd)
			{
				pAIActivityWnd->CreateWnd(GetHWND());
				pAIActivityWnd->ShowWindow();
			}

			KillTimer(GetRoot(), SHOW_ACTIVITY_TIMER);
		}
		else if (event.wParam == TIMER_EARTH_ID)
		{
			if (m_pAnimateControl == nullptr)
				return true;

			if (m_nCurFrame >= 99)
			{
				m_nCurFrame = 0;
			}

			TCHAR szPath[MAX_PATH];
			_stprintf_s(szPath, _T("#ai_animate_%d"), m_nCurFrame);
			m_pAnimateControl->SetAttribute(_T("bk.image"), szPath);
			m_pAnimateControl->GetParent()->Invalidate();

			m_nCurFrame++;
		}
	}

	return true;
}

void CAICabinWnd::OnCreate()
{
	SetWindowClassName(_T("AICabinWnd"));
	__super::OnCreate();

	m_pButtonTipsStart = dynamic_cast<CButtonUI*>(this->FindControl(_T("text_tips_start")));
	m_pAnimateControl = this->FindControl(_T("AIAnimateControl"));
    m_pButtonTipsEnd = dynamic_cast<CButtonUI*>(this->FindControl(_T("text_tips_end")));

    m_pLayHello = dynamic_cast<CLayoutUI*>(this->FindControl(_T("AIHelloLayout")));
    if (m_pLayHello)
        m_pLayHello->SetVisible(true);

    m_pLayEnd = dynamic_cast<CLayoutUI*>(this->FindControl(_T("AIEndLayout")));
    if (m_pLayEnd)
        m_pLayEnd->SetVisible(false);


	GetRoot()->OnEvent += MakeDelegate(this, &CAICabinWnd::OnTimer);

    CApplication::GetInstance()->m_pAICabinWnd = this;
}

void CAICabinWnd::OnClose()
{
	PostMessage(CApplication::GetInstance()->GetMainHwnd(), WM_QUIT, 0, 0);
    CApplication::GetInstance()->m_pAICabinWnd = nullptr;
}

bool CAICabinWnd::ShowWindow(int nCmdShow /*= SW_SHOW*/)
{
	bool bResult = __super::ShowWindow(nCmdShow);

	if (nCmdShow != SW_SHOW)
		return bResult;

	this->SetTimer(GetRoot(), SHOW_ACTIVITY_TIMER, SHOW_ACTIVITY_INTERVAL);

	this->SetTimer(GetRoot(), TIMER_EARTH_ID, TIMER_EARTH_TIME);

	return bResult;
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
/*        case VK_F6:
        {
            CAIViewResultsWnd* pAIViewResultsWnd = new CAIViewResultsWnd;
            if (pAIViewResultsWnd)
            {
                pAIViewResultsWnd->SetPercentage(0, 30);
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
                pAIViewResultsWnd->SetPercentage(35, 70);
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
                pAIViewResultsWnd->SetPercentage(100, 99);
                pAIViewResultsWnd->CreateWnd(GetHWND());
                pAIViewResultsWnd->ShowWindow();
            }
            break;
        }
		case VK_F9:
		{
			CAISpeakLearnWnd* pAISpeakLearnWnd = new CAISpeakLearnWnd;
			if (pAISpeakLearnWnd)
			{
				pAISpeakLearnWnd->CreateWnd(GetHWND());
				pAISpeakLearnWnd->ShowWindow();
			}
			break;
		}*/
		case VK_F4:
		{
			CAIFaceLearnWnd* pAIFaceLearnWnd = new CAIFaceLearnWnd;
			if (pAIFaceLearnWnd)
			{
				pAIFaceLearnWnd->CreateWnd(GetHWND());
				pAIFaceLearnWnd->ShowWindow();
				//pAIFaceLearnWnd->BeginFace();
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

void CAICabinWnd::ShowEndLayout()
{
    m_pLayHello = dynamic_cast<CLayoutUI*>(this->FindControl(_T("AIHelloLayout")));
    if (m_pLayHello)
        m_pLayHello->SetVisible(false);

    m_pLayEnd = dynamic_cast<CLayoutUI*>(this->FindControl(_T("AIEndLayout")));
    if (m_pLayEnd)
        m_pLayEnd->SetVisible(true);

    WCHAR szBuf[1024];
    _stprintf_s(szBuf, _countof(szBuf), I18NSTR(_T("#StrTipEnd")), CApplication::GetInstance()->GetUserNameW().c_str());
    if (m_pButtonTipsEnd)
    {
        m_pButtonTipsEnd->SetText(szBuf);
        m_pButtonTipsEnd->Invalidate();
    }

	wstring strEnd = I18NSTR(_T("#StrTipEndNow")) + wstring(szBuf);

	//语音合成播放
	CSpeechSynthControl::GetInstance()->ControlSpeechSynthStartLeave(strEnd);
}
