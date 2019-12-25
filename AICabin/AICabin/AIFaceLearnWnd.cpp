#include "stdafx.h"
#include "Singleton.h"
#include "AILearnBaseWnd.h"
#include "AIViewResultsWnd.h"
#include "AIFaceLearnWnd.h"
#include "AISpeakLearnWnd.h"
#include "AIActivityWnd.h"
#include "AICabinWnd.h"
#include "Application.h"

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

void CAIFaceLearnWnd::SetFaceQuestion(wstring strFaceQuestion, bool bSucc)
{
    m_bSuccess = bSucc;
	if (m_pBtnFaceCommonTitle
		&& m_pBtnFaceTitleTop)
	{
		WCHAR szBuf[1024];
		_stprintf_s(szBuf, _countof(szBuf), I18NSTR(_T("#StrFaceLearnCommonTitle")), strFaceQuestion.c_str());

		m_pBtnFaceCommonTitle->SetText(szBuf);
		m_pBtnFaceTitleBottom->SetText(szBuf);
    
        if (m_bSuccess)
            m_pBtnFaceTitleTop->SetText(I18NSTR(_T("#StrFaceLearnTitleTop")));
        else
            m_pBtnFaceTitleTop->SetText(I18NSTR(_T("#StrFaceLearnTitleTop2")));
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

    CApplication::GetInstance()->m_pAIFaceLearnWnd = this;
}

void CAIFaceLearnWnd::OnClose()
{
    CApplication::GetInstance()->m_pAIFaceLearnWnd = nullptr;
}

bool CAIFaceLearnWnd::OnEventReturn(TNotifyUI* pTNotify)
{
    //���ص���һ����
    CloseWindow();

    //������Ƭ��ʼѧϰ����
    if (CApplication::GetInstance()->m_pAISpeakLearnWnd != nullptr)
        (CApplication::GetInstance()->m_pAISpeakLearnWnd)->StartNewRecord();

    return true;
}

bool CAIFaceLearnWnd::OnEventLeave(TNotifyUI* pTNotify)
{
    //�ر��������鴰��
    CloseWindow();

    //�ر�ѧϰ��Ƭ����
    if (CApplication::GetInstance()->m_pAISpeakLearnWnd != nullptr)
        (CApplication::GetInstance()->m_pAISpeakLearnWnd)->CloseWindow();

    //�رջ�б���
    if (CApplication::GetInstance()->m_pAIActivityWnd != nullptr)
        (CApplication::GetInstance()->m_pAIActivityWnd)->CloseWindow();

    //������������
    if (CApplication::GetInstance()->m_pAICabinWnd != nullptr)
        (CApplication::GetInstance()->m_pAICabinWnd)->ShowEndLayout();

    return true;
}

bool CAIFaceLearnWnd::OnBtnViewResult(TNotifyUI* pTNotify)
{
    //���뵽�ɼ��������

    CAIViewResultsWnd* pAIViewResultsWnd = new CAIViewResultsWnd;
    if (pAIViewResultsWnd)
    {
        pAIViewResultsWnd->SetPercentage(0, 30);
        pAIViewResultsWnd->CreateWnd(GetHWND());
        pAIViewResultsWnd->ShowWindow();
    }

    return true;
}
