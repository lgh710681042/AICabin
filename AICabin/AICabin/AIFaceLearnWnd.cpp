#include "stdafx.h"
#include "AIFaceLearnWnd.h"


CAIFaceLearnWnd::CAIFaceLearnWnd()
{
}


CAIFaceLearnWnd::~CAIFaceLearnWnd()
{
}

HWND CAIFaceLearnWnd::CreateWnd(HWND hParent)
{
	SetLayerWindow(true);
	ModifyWndStyle(GWL_EXSTYLE, NULL, WS_EX_TOOLWINDOW);

	HWND hWnd = this->Create(hParent, _T("AIFaceLearnWnd"));
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

void CAIFaceLearnWnd::OnCreate()
{
	__super::OnCreate();
}

void CAIFaceLearnWnd::OnClose()
{

}
