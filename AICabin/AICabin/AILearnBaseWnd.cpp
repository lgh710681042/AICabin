#include "stdafx.h"
#include "AILearnBaseWnd.h"


CAILearnBaseWnd::CAILearnBaseWnd()
{
}


CAILearnBaseWnd::~CAILearnBaseWnd()
{
}

void CAILearnBaseWnd::OnCreate()
{
	__super::OnCreate();

	//◊Û”“¡Ω±ﬂ∞¥≈•√Ê∞Â
	m_pAILeftBtnPanel = dynamic_cast<CLayoutUI*> (FindControl(_T("AILeftBtnPanel")));
	m_pAILeftBtnPanelUnExpend = dynamic_cast<CLayoutUI*> (FindControl(_T("AILeftBtnPanelUnExpend")));
	m_pAIRightBtnPanel = dynamic_cast<CLayoutUI*> (FindControl(_T("AIRightBtnPanel")));
	m_pAIRightBtnPanelUnExpend = dynamic_cast<CLayoutUI*> (FindControl(_T("AIRightBtnPanelUnExpend")));
}

bool CAILearnBaseWnd::OnBtnLeftPanelArraw(TNotifyUI* pTNotify)
{
	if (m_pAILeftBtnPanel
		&& m_pAILeftBtnPanelUnExpend)
	{
		m_pAILeftBtnPanel->SetVisible(false);
		m_pAILeftBtnPanelUnExpend->SetVisible(true);
	}

	return true;
}

bool CAILearnBaseWnd::OnBtnLeftPanelArrawUn(TNotifyUI* pTNotify)
{
	if (m_pAILeftBtnPanel
		&& m_pAILeftBtnPanelUnExpend)
	{
		m_pAILeftBtnPanel->SetVisible(true);
		m_pAILeftBtnPanelUnExpend->SetVisible(false);
	}

	return true;
}

bool CAILearnBaseWnd::OnBtnRightPanelArraw(TNotifyUI* pTNotify)
{
	if (m_pAIRightBtnPanel
		&& m_pAIRightBtnPanelUnExpend)
	{
		m_pAIRightBtnPanel->SetVisible(false);
		m_pAIRightBtnPanelUnExpend->SetVisible(true);
	}

	return true;
}

bool CAILearnBaseWnd::OnBtnRightPanelArrawUn(TNotifyUI* pTNotify)
{
	if (m_pAIRightBtnPanel
		&& m_pAIRightBtnPanelUnExpend)
	{
		m_pAIRightBtnPanel->SetVisible(true);
		m_pAIRightBtnPanelUnExpend->SetVisible(false);
	}

	return true;
}
