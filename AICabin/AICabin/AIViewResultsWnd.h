#pragma once
#include "NewUI/UIAutoSizeButton.h"

class CAIViewResultsWnd
	: public CWindowUI
{
public:
    CAIViewResultsWnd();
    virtual ~CAIViewResultsWnd();

    UIBEGIN_MSG_MAP
        UI_EVENT_ID_HANDLER(UINOTIFY_CLICK, _T("btn_right_panel_return"), OnEventReturn);
        UI_EVENT_ID_HANDLER(UINOTIFY_CLICK, _T("btn_left_panel_return"), OnEventReturn);
        UI_EVENT_ID_HANDLER(UINOTIFY_CLICK, _T("btn_right_panel_leave"), OnEventLeave);
        UI_EVENT_ID_HANDLER(UINOTIFY_CLICK, _T("btn_left_panel_leave"), OnEventLeave);
        UI_EVENT_ID_HANDLER(UINOTIFY_CLICK, _T("btn_view_results_no"), OnEventNo);
        UI_EVENT_ID_HANDLER(UINOTIFY_CLICK, _T("btn_view_results_go"), OnEventGo);
    UIEND_MSG_MAP

	HWND			CreateWnd(HWND hParent);
    void			SetPercentage(int nScore, int nUseTime);

protected:
	//override super
	virtual void		OnCreate();
	virtual void		OnClose();

private:
    bool                OnEventReturn(TNotifyUI* pTNotify);//返回 响应事件
    bool                OnEventLeave(TNotifyUI* pTNotify);//离开 响应事件
    bool                OnEventNo(TNotifyUI* pTNotify);//不了 响应事件
    bool                OnEventGo(TNotifyUI* pTNotify);//好的，继续 响应事件

    void                UpdateViewData();//更新显示数据
    void                ShowCtrlStatusOfZero();//0分各控件状态
    void                ShowCtrlStatusOfOther();//[1,99]分各空间状态
    void                ShowCtrlStatusOfHundred();//100分各空间状态
    std::wstring        SecondsToTimeString(int nSeconds);//秒数转化为时分秒时间字符串

private:
    int                 m_nScore;//得分
    std::wstring        m_strUseTime;//用时

    CControlUI*         m_pCtrlPaint50;
    CControlUI*         m_pCtrlPaint100;
    CControlUI*         m_pCtrlIcon;
    CAutoSizeButtonUI*  m_pLabelPercentage;
    CAutoSizeButtonUI*  m_pLabelTimeData;
    CAutoSizeButtonUI*  m_pLabelEncourage;
};

