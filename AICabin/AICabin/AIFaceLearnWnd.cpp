#include "stdafx.h"
#include "AILearnBaseWnd.h"
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

void CAIFaceLearnWnd::SetFaceQuestion(wstring strFaceQuestion)
{
	if (m_pBtnFaceCommonTitle
		&& m_pBtnFaceTitleTop)
	{
		WCHAR szBuf[1024];
		_stprintf_s(szBuf, _countof(szBuf), I18NSTR(_T("#StrFaceLearnCommonTitle")), strFaceQuestion.c_str());

		m_pBtnFaceCommonTitle->SetText(szBuf);
		m_pBtnFaceTitleBottom->SetText(szBuf);
	}
}

void CAIFaceLearnWnd::OnCreate()
{
	__super::OnCreate();

	m_pBtnFaceCommonTitle = dynamic_cast<CButtonUI*> (FindControl(_T("btn_face_common_title")));
	m_pBtnFaceTitleTop = dynamic_cast<CButtonUI*> (FindControl(_T("btn_face_title_top")));
	m_pBtnFaceTitleBottom = dynamic_cast<CButtonUI*> (FindControl(_T("btn_face_title_bottom")));

	if (m_pAILeftBtnPanel
		&& m_pAILeftBtnPanelUnExpend)
	{
		m_pAILeftBtnPanel->SetVisible(true);
		m_pAILeftBtnPanelUnExpend->SetVisible(false);
	}
}

void CAIFaceLearnWnd::OnClose()
{

}
