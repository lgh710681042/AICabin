#pragma once

class CAICabinWnd 
	: public CWindowUI
{
public:
	CAICabinWnd();
	virtual ~CAICabinWnd();

	HWND				CreateWnd();

protected:
	//override super
	virtual void		OnCreate();
	virtual void		OnClose();

	virtual LRESULT		WindowProc(UINT message, WPARAM wParam, LPARAM lParam);

private:
	void				SwitchLayout();

private:
	
};

