#pragma once
class CAIActivityWnd
	: public CWindowUI
{
public:
	CAIActivityWnd();
	virtual ~CAIActivityWnd();

	HWND			CreateWnd(HWND hParent);

protected:
	//override super
	virtual void		OnCreate();
	virtual void		OnClose();


};

