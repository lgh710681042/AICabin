#pragma once

class CAILearnBaseWnd
	: public CWindowUI
{
public:
	CAILearnBaseWnd();
	virtual ~CAILearnBaseWnd();

protected:
	//override super
	virtual void		OnCreate();

protected:
	bool                OnBtnLeftPanelArraw(TNotifyUI* pTNotify);
	bool                OnBtnLeftPanelArrawUn(TNotifyUI* pTNotify);
	bool                OnBtnRightPanelArraw(TNotifyUI* pTNotify);
	bool                OnBtnRightPanelArrawUn(TNotifyUI* pTNotify);

protected:
	//�������߰�ť���
	CLayoutUI*			m_pAILeftBtnPanel = nullptr;
	CLayoutUI*			m_pAILeftBtnPanelUnExpend = nullptr;
	CLayoutUI*			m_pAIRightBtnPanel = nullptr;
	CLayoutUI*			m_pAIRightBtnPanelUnExpend = nullptr;
};

