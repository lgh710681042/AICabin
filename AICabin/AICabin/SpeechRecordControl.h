#pragma once

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

private:
	void*			m_pEngine = nullptr;
};

