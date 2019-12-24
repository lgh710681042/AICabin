#pragma once
#include <vector>
#include "NewUI/UIAutoSizeButton.h"

class CAISpeakLearnWnd
	: public CAILearnBaseWnd
{
public:
	CAISpeakLearnWnd();
	virtual ~CAISpeakLearnWnd();

    UIBEGIN_MSG_MAP
        UI_EVENT_ID_HANDLER(UINOTIFY_CLICK, _T("btn_left_panel_leave"), OnEventLeave);
        UI_EVENT_ID_HANDLER(UINOTIFY_CLICK, _T("btn_left_panel_return"), OnEventReturn);
        UI_EVENT_ID_HANDLER(UINOTIFY_CLICK, _T("btn_speak_learn_record"), OnEventSpeakLearnRecord);
        UI_EVENT_ID_HANDLER(UINOTIFY_CLICK, _T("btn_speak_jump"), OnEventReadJump);
        UI_EVENT_ID_HANDLER(UINOTIFY_CLICK, _T("btn_speak_read_again"), OnEventReadAgain);
		UI_EVENT_ID_HANDLER(UINOTIFY_CLICK, _T("btn_left_panel_arraw"), OnBtnLeftPanelArraw);
		UI_EVENT_ID_HANDLER(UINOTIFY_CLICK, _T("btn_left_panel_arraw_un"), OnBtnLeftPanelArrawUn);
		UI_EVENT_ID_HANDLER(UINOTIFY_CLICK, _T("btn_right_panel_arraw"), OnBtnRightPanelArraw);
		UI_EVENT_ID_HANDLER(UINOTIFY_CLICK, _T("btn_right_panel_arraw_un"), OnBtnRightPanelArrawUn);
    UIEND_MSG_MAP

	HWND			CreateWnd(HWND hParent);

protected:
	//override super
	virtual void		OnCreate();
	virtual void		OnClose();

	virtual LRESULT		WindowProc(UINT message, WPARAM wParam, LPARAM lParam);

private:
    bool				OnEventLeave(TNotifyUI* pTNotify); // 离开响应事件
    bool				OnEventReturn(TNotifyUI* pTNotify); // 返回响应事件
    bool                OnEventSpeakLearnRecord(TNotifyUI* pTNotify); // 麦克风响应事件
    bool                OnEventReadJump(TNotifyUI* pTNotify); // 跳过，朗读下一个
    bool                OnEventReadAgain(TNotifyUI* pTNotify); // 重新朗读

    bool                OnCheckRecordTime(TEventUI &evt);//涨条定时器
    void                ShowSpeakFailResult();//显示朗结果

	void				SetSpeakFailLayoutVisible(bool bVisible);		//识别失败layout显示隐藏

private:
    vector<std::wstring> m_vecExample;
    int m_nTotalTimeWidth;//涨条总长度
    int m_nEveryTimeWidth;//每次涨条长度
    int m_nTatalTimes;//总次数
    int m_nCurTimes;//当前次数
    RECT m_rc;

    CAutoSizeButtonUI*  m_pBtnCardText;
    CButtonUI*          m_pBtnSpeakLearnRecord;
	CAnimationUI*       m_pBtnSpeakLearnRecording;
    CButtonUI*          m_pBtnSpeakJump;
    CButtonUI*          m_pBtnSpeakReadAgain;
    CAutoSizeButtonUI*  m_pBtnSpeakTimesTips;
    CLayoutUI*          m_pLayoutSpeakLearnTime;

	int m_nSpeakTotalTimes = 3;				//speak 总次数
	int m_nSpeakCurTimes = 1;				//speak 当前次数
	wstring m_strSpeakQuestion = _T("");		//speak question
};

