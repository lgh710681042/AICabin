#pragma once

enum Resultsstate
{
    Resultsstate_Zero,
    Resultsstate_Hundred,
    Resultsstate_Other,
};

class CAIViewResultsWnd
	: public CWindowUI
{
public:
    CAIViewResultsWnd();
    virtual ~CAIViewResultsWnd();

	HWND			CreateWnd(HWND hParent);
    void			SetViewResultsState(Resultsstate eState) { m_eState = eState; }

protected:
	//override super
	virtual void		OnCreate();
	virtual void		OnClose();

    Resultsstate m_eState = Resultsstate_Hundred;
};

