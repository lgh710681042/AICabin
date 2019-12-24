#pragma once

struct SpeakResultStruct
{
	int nCode;
	wstring strMsgUnicode = _T("");
};
//

class CSpeechRecordControl 
	: public Util::Singleton<CSpeechRecordControl>
{
public:
	CSpeechRecordControl();
	~CSpeechRecordControl();

	bool			Init();

	bool			ControlSpeechRecoStart();

	bool			ControlSpeechRecoStop();

	void			SetMsgHwnd(HWND hMsgWnd){ m_hMsgWnd = hMsgWnd; };

	HWND			GetMsgHwnd(){ return m_hMsgWnd; }

private:
	void*			m_pEngine = nullptr;
	HWND			m_hMsgWnd = nullptr;
};

