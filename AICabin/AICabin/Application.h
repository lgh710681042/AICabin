#pragma once

class CMainWnd;
class CAICabinWnd;
class CAIActivityWnd;
class CAISpeakLearnWnd;
class CAIFaceLearnWnd;

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

	HWND	GetMainUIHwnd(){ return m_hUIWnd; }

	void	SetMainUIHwnd(HWND hwnd){ m_hUIWnd = hwnd; };

	wstring GetUserID(){ return m_strUserID; }

	wstring GetUserName(){ return m_strUserName; }

	void	SetRequest(bool bRequest){ m_bRequest = bRequest; };

	CRITICAL_SECTION		m_ControlLock;

	CRITICAL_SECTION		m_ControlLockFace;

	bool	OpenAICabin();

	bool	LeaveAICabin(bool bSync = false);

private:
	bool	InitDirectUI(HINSTANCE hInstance);


public:
    CAICabinWnd*	    m_pAICabinWnd = nullptr;
    CAIActivityWnd*     m_pAIActivityWnd = nullptr;//��б���
    CAISpeakLearnWnd*   m_pAISpeakLearnWnd = nullptr;//ѧϰ��Ƭ����
    CAIFaceLearnWnd*    m_pAIFaceLearnWnd = nullptr;//�������鴰��

private:
	HWND		m_hMsgWnd = nullptr;
	HWND		m_hUIWnd = nullptr;
	CMainWnd*	m_pMainWnd = nullptr;

	//�ж��Ƿ�ʼ��ѯ
	bool		m_bRequest = true;

	wstring		m_strUserID;
	wstring		m_strUserName;
};

