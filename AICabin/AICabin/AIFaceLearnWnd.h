#pragma once
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
	UIEND_MSG_MAP

	HWND			CreateWnd(HWND hParent);

    void			SetFaceQuestion(wstring strFaceQuestion, bool bSucc);

protected:
	//override super
	virtual void		OnCreate();
	virtual void		OnClose();

protected:
    bool                OnEventReturn(TNotifyUI* pTNotify);//���� ��Ӧ�¼�
    bool                OnEventLeave(TNotifyUI* pTNotify);//�뿪 ��Ӧ�¼�
    bool                OnBtnViewResult(TNotifyUI* pTNotify);//�鿴�ɼ� ��Ӧ�¼�

private:
	wstring				m_strFaceQuestion = _T("");
    bool                m_bSuccess = true;//�ʶ��ɹ������ʶ�ʧ��3��

	CButtonUI* m_pBtnFaceCommonTitle = nullptr;
	CButtonUI* m_pBtnFaceTitleTop = nullptr;
	CButtonUI* m_pBtnFaceTitleBottom = nullptr;
};

