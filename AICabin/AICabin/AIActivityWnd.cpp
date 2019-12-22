#include "stdafx.h"
#include "AIActivityWnd.h"


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
}

void CAIActivityWnd::OnClose()
{

}
