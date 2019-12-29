#pragma once
#include <vector>
#include <map>
#include "NewUI/UIAutoSizeButton.h"

class CAISpeakLearnWnd
	: public CAILearnBaseWnd
{
public:
	CAISpeakLearnWnd();
	virtual ~CAISpeakLearnWnd();

    UIBEGIN_MSG_MAP
        UI_EVENT_ID_HANDLER(UINOTIFY_CLICK, _T("btn_right_panel_return"), OnEventReturn);
        UI_EVENT_ID_HANDLER(UINOTIFY_CLICK, _T("btn_left_panel_return"), OnEventReturn);
        UI_EVENT_ID_HANDLER(UINOTIFY_CLICK, _T("btn_right_panel_leave"), OnEventLeave);
        UI_EVENT_ID_HANDLER(UINOTIFY_CLICK, _T("btn_left_panel_leave"), OnEventLeave);
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

public:
    void                StartNewRecord();//��ʼ�µĿ�Ƭʶ��
    time_t              GetBeginTime() { return m_tBeginTime; }
private:
    bool				OnEventLeave(TNotifyUI* pTNotify); // �뿪��Ӧ�¼�
    bool				OnEventReturn(TNotifyUI* pTNotify); // ������Ӧ�¼�
    bool                OnEventSpeakLearnRecord(TNotifyUI* pTNotify); // ��˷���Ӧ�¼�
    bool                OnEventReadJump(TNotifyUI* pTNotify); // �������ʶ���һ��
    bool                OnEventReadAgain(TNotifyUI* pTNotify); // �����ʶ�

    bool                OnCheckRecordTime(TEventUI &evt);//������ʱ��
    void                ShowSpeakFailResult();//��ʾ�ʽ��

	void				SetSpeakFailLayoutVisible(bool bVisible);//ʶ��ʧ��layout��ʾ����
    void                SaveHaveReadData();//��¼�Ѿ������ĵ���
    void                GetNewSpeakQuestion();//��תһ���¿�Ƭ
    bool                CheckSpeakQuestionAccRate(const wstring& strResult, const wstring& strSrc);//��֤׼ȷ��
    void                SpeechTitle();//�������ű���
private:
    vector<std::wstring> m_vecExample;
    map<std::wstring, int> m_mapHaveRead;
    int m_nTotalTimeWidth = 0;//�����ܳ���
    int m_nEveryTimeWidth = 0;//ÿ����������
    int m_nCurTimes = 0;//��ǰ����
    RECT m_rc;
    time_t m_tBeginTime = 0;//��ʼʱ��

    CAutoSizeButtonUI*  m_pBtnCardText = nullptr;
    CButtonUI*          m_pBtnSpeakLearnRecord = nullptr;
    CAnimationUI*       m_pBtnSpeakLearnRecording = nullptr;
    CButtonUI*          m_pBtnSpeakJump = nullptr;
    CButtonUI*          m_pBtnSpeakReadAgain = nullptr;
    CAutoSizeButtonUI*  m_pBtnSpeakTimesTips = nullptr;
    CLayoutUI*          m_pLayoutSpeakLearnTime = nullptr;
    CLayoutUI*          m_pLayoutSpeakLearnCard = nullptr;
    CControlUI*         m_pCtrlSpreadOne = nullptr;
    CControlUI*         m_pCtrlSpreadTwo = nullptr;
    CAutoSizeButtonUI*  m_pLabelSpeakTitle = nullptr;

	int m_nSpeakTotalTimes = 3;				//speak �ܴ���
	int m_nSpeakCurTimes = 1;				//speak ��ǰ����
	wstring m_strSpeakQuestion = _T("");		//speak question
};

