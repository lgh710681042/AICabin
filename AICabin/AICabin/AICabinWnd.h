#pragma once

class CAICabinWnd 
	: public CWindowUI
{
public:
	CAICabinWnd();
	virtual ~CAICabinWnd();

	HWND				CreateWnd();

	void				SetFullScreen(bool fullScreen);

	void				MarkFullScreen(bool fullscreen);

	wstring				SetTipsStart(wstring& strUserName);

	virtual bool		ShowWindow(int nCmdShow = SW_SHOW);

    void                ShowEndLayout();//œ‘ æΩ· ¯layout

protected:
	bool					OnTimer(TEventUI& event);

	//override super
	virtual void		OnCreate();
	virtual void		OnClose();

	virtual LRESULT		WindowProc(UINT message, WPARAM wParam, LPARAM lParam);

private:
	void				SwitchLayout();

private:
	// Information saved before going into fullscreen mode, used to restore the
	// window afterwards.
	struct SavedWindowInfo {
		LONG style;
		LONG ex_style;
		RECT window_rect;
	};

	// Saved window information from before entering fullscreen mode.
	SavedWindowInfo saved_window_info_;

	CComPtr<ITaskbarList2>  task_bar_list_;
	bool                    fullscreen_ = false;
	int	                    m_nCurFrame = 0;

	CButtonUI*              m_pButtonTipsStart = nullptr;
	CControlUI*             m_pAnimateControl = nullptr;
    CButtonUI*              m_pButtonTipsEnd = nullptr;
    CLayoutUI*              m_pLayHello = nullptr;
    CLayoutUI*              m_pLayEnd = nullptr;
};

