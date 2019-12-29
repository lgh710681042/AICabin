#pragma once

enum UIFaceType
{
	FaceInitStype,		//初始状态
	FaceFailRetry,		//失败了重试
	FaceFailViewResult,	//失败了，不要灰心，先看看本次成绩
	SuccessViewResult,	//成功了，查看本次成绩
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

	virtual LRESULT		WindowProc(UINT message, WPARAM wParam, LPARAM lParam); //窗口过程函数

protected:
	bool				OnTimer(TEventUI& event);

    bool                OnEventReturn(TNotifyUI* pTNotify);//返回 响应事件
    bool                OnEventLeave(TNotifyUI* pTNotify);//离开 响应事件
    bool                OnBtnViewResult(TNotifyUI* pTNotify);//查看成绩 响应事件

	bool				OnBtnFaceBegin(TNotifyUI* pTNotify);	//开始识别
	bool				OnBtnFaceNoRetry(TNotifyUI* pTNotify);	//不重新识别
	bool				OnBtnFaceRetry(TNotifyUI* pTNotify);	//不重新识别

    void                HideBelowControl();//识别过程中隐藏下面部分控件
private:
	wstring				m_strFaceQuestion = _T("");
    bool                m_bSuccess = true;//朗读成功或者朗读失败3次
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
	CLayoutUI* m_pFaceRecongnition = nullptr;		//圆圈布局

	int m_nFaceTotalTimes = 3;				//face 总次数
	int m_nFaceCurTimes = 1;				//face 当前次数
    int m_nCurFaceAnimateFrame = 0;         //face 动效 当前次数
};

