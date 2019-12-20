#pragma once
class CApplication 
	: public Util::Singleton<CApplication>
{
public:
	CApplication();
	~CApplication();

public:
	bool	Initialize(HINSTANCE hInstance, LPTSTR lpCmdLine);

	bool	Run();

private:
	bool	InitDirectUI(HINSTANCE hInstance);

private:
	HWND		m_hMsgWnd = nullptr;
	CMainWnd*	m_pMainWnd = nullptr;
};

