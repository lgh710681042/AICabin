#include "stdafx.h"
#include "Singleton.h"
#include "CommonUtil.h"
#include "SpeechTechControl.h"
#include "SpeechRecordControl.h"

void __stdcall onResult(int nCode, char *strMsg)
{
	if ( nCode == 0 
		&& strMsg)
	{
		wstring strMsgUnicode = CommonUtil::Utf8ToUnicode(strMsg);

		logwrapper::OutputInfo("{} strMsg: {}", __FUNCTION__, strMsg);

		SpeakResultStruct* pSpeakResult = new SpeakResultStruct;
		pSpeakResult->nCode = nCode;
		pSpeakResult->strMsgUnicode = strMsgUnicode;

		
		if (::PostMessage(CSpeechRecordControl::GetInstance()->GetMsgHwnd(),
			WM_SPEAK_RESULT,
			NULL,
			LPARAM(pSpeakResult)) == FALSE)
		{
			delete pSpeakResult;
		}
	}
	else
	{
		::PostMessage(CSpeechRecordControl::GetInstance()->GetMsgHwnd(),
			WM_SPEAK_RESULT,
			NULL,
			NULL);
	}
}

void __stdcall onVolume(int nVolume)
{

}


CSpeechRecordControl::CSpeechRecordControl()
{
	Init();
}


CSpeechRecordControl::~CSpeechRecordControl()
{
}

bool CSpeechRecordControl::Init()
{
	m_pEngine = CSpeechTechControl::GetInstance()->SpeechRecoInit(onResult, onVolume, true, "", "", "", DEV_PID::EN);

	return m_pEngine == nullptr;
}

bool CSpeechRecordControl::ControlSpeechRecoStart()
{
	return CSpeechTechControl::GetInstance()->SpeechRecoStart(m_pEngine);
}

bool CSpeechRecordControl::ControlSpeechRecoStop()
{
	return CSpeechTechControl::GetInstance()->SpeechRecoStop(m_pEngine);
}
