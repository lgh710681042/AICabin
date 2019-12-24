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
    bool				OnEventLeave(TNotifyUI* pTNotify); // �뿪��Ӧ�¼�
    bool				OnEventReturn(TNotifyUI* pTNotify); // ������Ӧ�¼�
    bool                OnEventSpeakLearnRecord(TNotifyUI* pTNotify); // ��˷���Ӧ�¼�
    bool                OnEventReadJump(TNotifyUI* pTNotify); // �������ʶ���һ��
    bool                OnEventReadAgain(TNotifyUI* pTNotify); // �����ʶ�

    bool                OnCheckRecordTime(TEventUI &evt);//������ʱ��
    void                ShowSpeakFailResult();//��ʾ�ʽ��

	void				SetSpeakFailLayoutVisible(bool bVisible);		//ʶ��ʧ��layout��ʾ����

private:
    vector<std::wstring> m_vecExample;
    int m_nTotalTimeWidth;//�����ܳ���
    int m_nEveryTimeWidth;//ÿ����������
    int m_nTatalTimes;//�ܴ���
    int m_nCurTimes;//��ǰ����
    RECT m_rc;

    CAutoSizeButtonUI*  m_pBtnCardText;
    CButtonUI*          m_pBtnSpeakLearnRecord;
	CAnimationUI*       m_pBtnSpeakLearnRecording;
    CButtonUI*          m_pBtnSpeakJump;
    CButtonUI*          m_pBtnSpeakReadAgain;
    CAutoSizeButtonUI*  m_pBtnSpeakTimesTips;
    CLayoutUI*          m_pLayoutSpeakLearnTime;

	int m_nSpeakTotalTimes = 3;				//speak �ܴ���
	int m_nSpeakCurTimes = 1;				//speak ��ǰ����
	wstring m_strSpeakQuestion = _T("");		//speak question
};

