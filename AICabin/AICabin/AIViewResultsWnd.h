#pragma once
#include "NewUI/UIAutoSizeButton.h"

class CAIViewResultsWnd
	: public CAILearnBaseWnd
{
public:
    CAIViewResultsWnd();
    virtual ~CAIViewResultsWnd();

    UIBEGIN_MSG_MAP
        UI_EVENT_ID_HANDLER(UINOTIFY_CLICK, _T("btn_view_results_return"), OnEventReturn);
        UI_EVENT_ID_HANDLER(UINOTIFY_CLICK, _T("btn_right_panel_return"), OnEventReturn);
        UI_EVENT_ID_HANDLER(UINOTIFY_CLICK, _T("btn_left_panel_return"), OnEventReturn);
        UI_EVENT_ID_HANDLER(UINOTIFY_CLICK, _T("btn_view_results_leave"), OnEventLeave);
        UI_EVENT_ID_HANDLER(UINOTIFY_CLICK, _T("btn_right_panel_leave"), OnEventLeave);
        UI_EVENT_ID_HANDLER(UINOTIFY_CLICK, _T("btn_left_panel_leave"), OnEventLeave);
        UI_EVENT_ID_HANDLER(UINOTIFY_CLICK, _T("btn_view_results_no"), OnEventNo);
        UI_EVENT_ID_HANDLER(UINOTIFY_CLICK, _T("btn_view_results_go"), OnEventGo);
		UI_EVENT_ID_HANDLER(UINOTIFY_CLICK, _T("btn_left_panel_arraw"), OnBtnLeftPanelArraw);
		UI_EVENT_ID_HANDLER(UINOTIFY_CLICK, _T("btn_left_panel_arraw_un"), OnBtnLeftPanelArrawUn);
		UI_EVENT_ID_HANDLER(UINOTIFY_CLICK, _T("btn_right_panel_arraw"), OnBtnRightPanelArraw);
		UI_EVENT_ID_HANDLER(UINOTIFY_CLICK, _T("btn_right_panel_arraw_un"), OnBtnRightPanelArrawUn);
    UIEND_MSG_MAP

	HWND			CreateWnd(HWND hParent);
    void			SetPercentage(int nScore, int nUseTime);

protected:
	//override super
	virtual void		OnCreate();
	virtual void		OnClose();

private:
    bool                OnEventReturn(TNotifyUI* pTNotify);//���� ��Ӧ�¼�
    bool                OnEventLeave(TNotifyUI* pTNotify);//�뿪 ��Ӧ�¼�
    bool                OnEventNo(TNotifyUI* pTNotify);//���� ��Ӧ�¼�
    bool                OnEventGo(TNotifyUI* pTNotify);//�õģ����� ��Ӧ�¼�

    void                UpdateViewData();//������ʾ����
    void                ShowCtrlStatusOfZero();//0�ָ��ؼ�״̬
    void                ShowCtrlStatusOfOther();//[1,99]�ָ��ռ�״̬
    void                ShowCtrlStatusOfHundred();//100�ָ��ռ�״̬
    std::wstring        SecondsToTimeString(int nSeconds);//����ת��Ϊʱ����ʱ���ַ���

private:
    int                 m_nScore;//�÷�
    std::wstring        m_strUseTime;//��ʱ

    CButtonUI*          m_pBtnReturn;
    CButtonUI*          m_pBtnLeave;
    CControlUI*         m_pCtrlPaint50;
    CControlUI*         m_pCtrlPaint100;
    CControlUI*         m_pCtrlIcon;
    CAutoSizeButtonUI*  m_pLabelPercentage;
    CAutoSizeButtonUI*  m_pLabelTimeData;
    CAutoSizeButtonUI*  m_pLabelEncourage;
    CLayoutUI*          m_pLayLeftBtnPanel;
    CLayoutUI*          m_pLayRightBtnPanel;
};

