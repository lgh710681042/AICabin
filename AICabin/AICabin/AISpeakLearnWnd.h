#pragma once
#include <vector>
#include "NewUI/UIAutoSizeButton.h"

class CAISpeakLearnWnd
	: public CWindowUI
{
public:
	CAISpeakLearnWnd();
	virtual ~CAISpeakLearnWnd();

    UIBEGIN_MSG_MAP
        UI_EVENT_ID_HANDLER(UINOTIFY_CLICK, _T("btn_left_panel_leave"), OnEventLeave);
        UI_EVENT_ID_HANDLER(UINOTIFY_CLICK, _T("btn_left_panel_return"), OnEventReturn);
        UI_EVENT_ID_HANDLER(UINOTIFY_CLICK, _T("btn_speak_learn_record"), OnEventSpeakLearnRecord);
    UIEND_MSG_MAP

	HWND			CreateWnd(HWND hParent);

protected:
	//override super
	virtual void		OnCreate();
	virtual void		OnClose();

private:
    bool				OnEventLeave(TNotifyUI* pTNotify); // �뿪��Ӧ�¼�
    bool				OnEventReturn(TNotifyUI* pTNotify); // ������Ӧ�¼�
    bool                OnEventSpeakLearnRecord(TNotifyUI* pTNotify); // ��˷���Ӧ�¼�


    bool                OnCheckRecordTime(TEventUI &evt);//������ʱ��

private:
    vector<std::string> m_vecExample;
    int m_nTotalTimeWidth;//�����ܳ���
    int m_nEveryTimeWidth;//ÿ����������
    int m_nTatalTimes;//�ܴ���
    int m_nCurTimes;//��ǰ����
    RECT m_rc;

    CAutoSizeButtonUI*  m_pBtnCardText;
    CButtonUI*          m_pBtnSpeakLearnRecord;
    CButtonUI*          m_pBtnSpeakLearnRecording;
    CLayoutUI*          m_pLayoutSpeakLearnTime;
};

