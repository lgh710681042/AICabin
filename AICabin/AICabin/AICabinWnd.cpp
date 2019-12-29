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
//#ifdef _DEBUG
//#define SHOW_ACTIVITY_INTERVAL		1000
//#else
#define SHOW_ACTIVITY_INTERVAL		5000
//#endif


//离开界面到欢迎界面定时器
#define SHOW_RE_WELCOME_TIMER		1002
#define SHOW_RE_WELCOME_INTERVAL	7000

//圆圈转动定时器
#define TIMER_EARTH_ID			    1003	// 转动定时器
#define TIMER_EARTH_TIME		    50		// 每66ms切换图片

//离开界面到待机页面
#define END_TO_WAIT_TIMER		    1004
#define END_TO_WAIT_INTERVAL		10000

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
		else if (event.wParam == SHOW_RE_WELCOME_TIMER)
		{
			KillTimer(GetRoot(), SHOW_RE_WELCOME_TIMER);

			CApplication::GetInstance()->SetRequest(true);
			CApplication::GetInstance()->OpenAICabin();
		}
        else if (event.wParam == END_TO_WAIT_TIMER)
        {
            KillTimer(GetRoot(), END_TO_WAIT_TIMER);

            if (m_pButtonTipsStart)
                m_pButtonTipsStart->SetVisible(false);

            if (m_pButtonTipsEnd)
                m_pButtonTipsEnd->SetVisible(false);
        }
	}

	return true;
}

void CAICabinWnd::OnCreate()
{
	SetWindowClassName(_T("AICabinWnd"));
	__super::OnCreate();

	m_pButtonTipsStart = dynamic_cast<CButtonUI*>(this->FindControl(_T("text_tips_start")));
    if (m_pButtonTipsStart)
        m_pButtonTipsStart->SetVisible(true);

	m_pAnimateControl = this->FindControl(_T("AIAnimateControl"));
    m_pButtonTipsEnd = dynamic_cast<CButtonUI*>(this->FindControl(_T("text_tips_end")));
    if (m_pButtonTipsEnd)
        m_pButtonTipsEnd->SetVisible(false);

    m_pLayHello = dynamic_cast<CLayoutUI*>(this->FindControl(_T("AIHelloLayout")));
    if (m_pLayHello)
        m_pLayHello->SetAttribute(_T("bk.image"), _T("#ai_hello_bg"));

	GetRoot()->OnEvent += MakeDelegate(this, &CAICabinWnd::OnTimer);

    CApplication::GetInstance()->m_pAICabinWnd = this;
}

void CAICabinWnd::OnClose()
{
	PostMessage(CApplication::GetInstance()->GetMainHwnd(), WM_QUIT, 0, 0);
    CApplication::GetInstance()->m_pAICabinWnd = nullptr;
}

bool CAICabinWnd::ShowWindow(int nCmdShow /*= SW_SHOW*/, bool bActivity /*= false*/)
{
	bool bResult = __super::ShowWindow(nCmdShow);

	if (nCmdShow != SW_SHOW)
		return bResult;

	if (bActivity)
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
		case VK_F4:
		{
			CAIFaceLearnWnd* pAIFaceLearnWnd = new CAIFaceLearnWnd;
			if (pAIFaceLearnWnd)
			{
				pAIFaceLearnWnd->CreateWnd(GetHWND());
				pAIFaceLearnWnd->SetFaceQuestion(_T("happy"), true);
				pAIFaceLearnWnd->ShowWindow();
				pAIFaceLearnWnd->BeginFace();
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
    if (m_pButtonTipsStart)
    {
        m_pButtonTipsStart->SetVisible(true);
        m_pButtonTipsStart->SetText(I18NSTR(_T("#StrTipEndNow")));
        m_pButtonTipsStart->Invalidate();
    }

    WCHAR szBuf[1024];
    _stprintf_s(szBuf, _countof(szBuf), I18NSTR(_T("#StrTipEnd")), CApplication::GetInstance()->GetUserName().c_str());
    if (m_pButtonTipsEnd)
    {
        m_pButtonTipsEnd->SetVisible(true);
        m_pButtonTipsEnd->SetText(szBuf);
        m_pButtonTipsEnd->Invalidate();    
    }

    if (m_pLayHello)
    {
        m_pLayHello->SetAttribute(_T("bk.image"), _T("#ai_end_bg"));
        m_pLayHello->GetParent()->Invalidate();
    }

    KillTimer(GetRoot(), TIMER_EARTH_ID);
    this->SetTimer(GetRoot(), TIMER_EARTH_ID, TIMER_EARTH_TIME);//光圈动效

	wstring strEnd = I18NSTR(_T("#StrTipEndNow")) + wstring(szBuf);
	//语音合成播放
	CSpeechSynthControl::GetInstance()->ControlSpeechSynthStartLeave(strEnd);

	this->SetTimer(GetRoot(), SHOW_RE_WELCOME_TIMER, SHOW_RE_WELCOME_INTERVAL);
	this->SetTimer(GetRoot(), END_TO_WAIT_TIMER, END_TO_WAIT_INTERVAL);

	//通知服务端，已退出学习舱
	//CApplication::GetInstance()->LeaveAICabin();
}

void CAICabinWnd::ShowBeginLayout()
{
    KillTimer(GetRoot(), SHOW_RE_WELCOME_TIMER);
    KillTimer(GetRoot(), END_TO_WAIT_TIMER);

    if (m_pButtonTipsEnd)
        m_pButtonTipsEnd->SetVisible(false);

	WCHAR szBuf[1024];
	_stprintf_s(szBuf, _countof(szBuf), I18NSTR(_T("#StrTipStart")), CApplication::GetInstance()->GetUserName().c_str());

	if (m_pButtonTipsStart)
	{
        m_pButtonTipsStart->SetVisible(true);
		m_pButtonTipsStart->SetText(szBuf);
		m_pButtonTipsStart->Invalidate();
	}

    if (m_pLayHello)
    {
        m_pLayHello->SetAttribute(_T("bk.image"), _T("#ai_hello_bg"));
        m_pLayHello->GetParent()->Invalidate();
    }
}

void CAICabinWnd::HideAIHelloLayout()
{
	if (m_pButtonTipsStart)
		m_pButtonTipsStart->SetVisible(false);

	if (m_pButtonTipsEnd)
		m_pButtonTipsEnd->SetVisible(false);
}

