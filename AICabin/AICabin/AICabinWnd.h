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

protected:
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

	CComPtr<ITaskbarList2> task_bar_list_;
	bool fullscreen_ = false;
};

