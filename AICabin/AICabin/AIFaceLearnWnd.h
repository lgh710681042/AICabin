#pragma once
class CAIFaceLearnWnd
	: public CWindowUI
{
public:
	CAIFaceLearnWnd();
	virtual ~CAIFaceLearnWnd();

	HWND			CreateWnd(HWND hParent);

protected:
	//override super
	virtual void		OnCreate();
	virtual void		OnClose();
};

