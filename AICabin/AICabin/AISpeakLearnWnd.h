#pragma once
class CAISpeakLearnWnd
	: public CWindowUI
{
public:
	CAISpeakLearnWnd();
	virtual ~CAISpeakLearnWnd();

	HWND			CreateWnd(HWND hParent);

protected:
	//override super
	virtual void		OnCreate();
	virtual void		OnClose();
};

