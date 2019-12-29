#pragma once

enum UIFaceType
{
	FaceInitStype,		//��ʼ״̬
	FaceFailRetry,		//ʧ��������
	FaceFailViewResult,	//ʧ���ˣ���Ҫ���ģ��ȿ������γɼ�
	SuccessViewResult,	//�ɹ��ˣ��鿴���γɼ�
};

class CAIFaceLearnWnd
	: public CAILearnBaseWnd
{
public:
	CAIFaceLearnWnd();
	virtual ~CAIFaceLearnWnd();

	UIBEGIN_MSG_MAP
		UI_EVENT_ID_HANDLER(UINOTIFY_CLICK, _T("btn_left_panel_arraw"), OnBtnLeftPanelArraw);
		UI_EVENT_ID_HANDLER(UINOTIFY_CLICK, _T("btn_left_panel_arraw_un"), OnBtnLeftPanelArrawUn);
		UI_EVENT_ID_HANDLER(UINOTIFY_CLICK, _T("btn_right_panel_arraw"), OnBtnRightPanelArraw);
		UI_EVENT_ID_HANDLER(UINOTIFY_CLICK, _T("btn_right_panel_arraw_un"), OnBtnRightPanelArrawUn);
        UI_EVENT_ID_HANDLER(UINOTIFY_CLICK, _T("btn_right_panel_return"), OnEventReturn);
        UI_EVENT_ID_HANDLER(UINOTIFY_CLICK, _T("btn_left_panel_return"), OnEventReturn);
        UI_EVENT_ID_HANDLER(UINOTIFY_CLICK, _T("btn_right_panel_leave"), OnEventLeave);
        UI_EVENT_ID_HANDLER(UINOTIFY_CLICK, _T("btn_left_panel_leave"), OnEventLeave);
        UI_EVENT_ID_HANDLER(UINOTIFY_CLICK, _T("btn_face_view_result"), OnBtnViewResult);
		UI_EVENT_ID_HANDLER(UINOTIFY_CLICK, _T("btn_face_begin"), OnBtnFaceBegin);
		UI_EVENT_ID_HANDLER(UINOTIFY_CLICK, _T("btn_face_no_retry"), OnBtnFaceNoRetry);
		UI_EVENT_ID_HANDLER(UINOTIFY_CLICK, _T("btn_face_retry"), OnBtnFaceRetry);
	UIEND_MSG_MAP

	HWND			CreateWnd(HWND hParent);

    void			SetFaceQuestion(wstring strFaceQuestion, bool bSucc);

	void			BeginFace();

	void			Switch(UIFaceType uiFaceType);

protected:
	//override super
	virtual void		OnCreate();
	virtual void		OnClose();

	virtual LRESULT		WindowProc(UINT message, WPARAM wParam, LPARAM lParam); //���ڹ��̺���

protected:
	bool				OnTimer(TEventUI& event);

    bool                OnEventReturn(TNotifyUI* pTNotify);//���� ��Ӧ�¼�
    bool                OnEventLeave(TNotifyUI* pTNotify);//�뿪 ��Ӧ�¼�
    bool                OnBtnViewResult(TNotifyUI* pTNotify);//�鿴�ɼ� ��Ӧ�¼�

	bool				OnBtnFaceBegin(TNotifyUI* pTNotify);	//��ʼʶ��
	bool				OnBtnFaceNoRetry(TNotifyUI* pTNotify);	//������ʶ��
	bool				OnBtnFaceRetry(TNotifyUI* pTNotify);	//������ʶ��

    void                HideBelowControl();//ʶ��������������沿�ֿؼ�
private:
	wstring				m_strFaceQuestion = _T("");
    bool                m_bSuccess = true;//�ʶ��ɹ������ʶ�ʧ��3��
	bool				m_bFaceSuccess = false;

	CButtonUI* m_pBtnFaceCommonTitle = nullptr;
	CButtonUI* m_pBtnFaceTitleTop = nullptr;
	CButtonUI* m_pBtnFaceTitleBottom = nullptr;
	CLayoutUI* m_pFaceLayout = nullptr;
	CLayoutUI* m_pFaceTitleTwo = nullptr;
	CButtonUI* m_pBtnFaceBegin = nullptr;
	CButtonUI* m_pBtnFaceViewResult = nullptr;
	CButtonUI* m_pBtnFaceTitleRetry = nullptr;
	CLayoutUI* m_pFaceFailLayout = nullptr;
    CControlUI* m_pFaceAnimateControl = nullptr;
	CLayoutUI* m_pFaceRecongnition = nullptr;		//ԲȦ����

	int m_nFaceTotalTimes = 3;				//face �ܴ���
	int m_nFaceCurTimes = 1;				//face ��ǰ����
    int m_nCurFaceAnimateFrame = 0;         //face ��Ч ��ǰ����
};

