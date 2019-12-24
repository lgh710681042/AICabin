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

	HWND	GetMainHwnd();

	wstring GetUserID(){ return m_strUserID; }

	wstring GetUserName(){ return m_strUserName; }

private:
	bool	InitDirectUI(HINSTANCE hInstance);

	bool	OpenAICabin();

private:
	HWND		m_hMsgWnd = nullptr;
	CMainWnd*	m_pMainWnd = nullptr;

	//ÅÐ¶ÏÊÇ·ñ¿ªÊ¼ÂÖÑ¯
	bool		m_bRequest = true;

	wstring		m_strUserID;
	wstring		m_strUserName;
};

