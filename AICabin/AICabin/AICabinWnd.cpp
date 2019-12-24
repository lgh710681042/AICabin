#include "stdafx.h"
#include "Singleton.h"
#include "AIActivityWnd.h"
#include "AISpeakLearnWnd.h"
#include "AIFaceLearnWnd.h"
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
			CAISpeakLearnWnd* pAISpeakLearnWnd = new CAISpeakLearnWnd;
			if (pAISpeakLearnWnd)
			{
				pAISpeakLearnWnd->CreateWnd(GetHWND());
				pAISpeakLearnWnd->ShowWindow();
			}
			break;
		}
		case VK_F7:
		{
			CAIFaceLearnWnd* pAIFaceLearnWnd = new CAIFaceLearnWnd;
			if (pAIFaceLearnWnd)
			{
				pAIFaceLearnWnd->CreateWnd(GetHWND());
				pAIFaceLearnWnd->ShowWindow();
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
