#include "stdafx.h"
#include "Singleton.h"
#include "CommonUtil.h"
#include "SpeechTechControl.h"


CSpeechTechControl::CSpeechTechControl()
{
	InitModule();
}


CSpeechTechControl::~CSpeechTechControl()
{
}

bool CSpeechTechControl::InitModule()
{
	wstring strSpeechTechPath = CommonUtil::GetLocalPath() + _T("\\SpeechTech.dll");
	m_hSpeechTech = LoadLibraryEx(strSpeechTechPath.c_str(), NULL, LOAD_WITH_ALTERED_SEARCH_PATH);
	if (m_hSpeechTech == nullptr)
	{
		//接口轮询测试
		logwrapper::OutputInfo("{} LoadLibrary error {}", __FUNCTION__, GetLastError());

		return false;
	}
		
	return true;
}

// 语音合成
bool CSpeechTechControl::SpeechSynthInit(bool bInlineBtAcount, const char app_id[], const char api_key[], const char secret_key[])
{
	if (m_hSpeechTech == nullptr)
		return false;

	if (funSpeechSynthInit_ == nullptr)
	{
		funSpeechSynthInit_ = (LPEx_SpeechSynthInit)GetProcAddress(m_hSpeechTech, "Ex_SpeechSynthInit");
	}

	if (funSpeechSynthInit_)
		return funSpeechSynthInit_(bInlineBtAcount, app_id, api_key, secret_key);

	return false;
}

bool CSpeechTechControl::SpeechSynthStartAsyn(const char *strText, const char *strFilePath, const char *strper /*= "0"*/, onResultCallback CbResult /*= NULL*/)
{
	if (m_hSpeechTech == nullptr)
		return false;

	if (funSpeechSynthStartAsyn_ == nullptr)
	{
		funSpeechSynthStartAsyn_ = (LPEx_SpeechSynthStartAsyn)GetProcAddress(m_hSpeechTech, "Ex_SpeechSynthStartAsyn");
	}

	if (funSpeechSynthStartAsyn_)
		return funSpeechSynthStartAsyn_(strText, strFilePath, strper, CbResult);

	return false;
}

void CSpeechTechControl::SpeechSynthUnInit()
{
	if (m_hSpeechTech == nullptr)
		return;

	if (funSpeechSynthUnInit_ == nullptr)
	{
		funSpeechSynthUnInit_ = (LPEx_SpeechSynthUnInit)GetProcAddress(m_hSpeechTech, "Ex_SpeechSynthUnInit");
	}

	if (funSpeechSynthUnInit_)
		return funSpeechSynthUnInit_();
}

// 语音识别
void* CSpeechTechControl::SpeechRecoInit(onResultCallback CbResult, onVolumeCallback CbVolume, bool bInlineBtAcount /*= true*/, const char *app_id /*= ""*/, const char *api_key /*= ""*/, const char *secret_key /*= ""*/, DEV_PID dev_pid /*= CN*/)
{
	if (m_hSpeechTech == nullptr)
		return nullptr;

	if (funSpeechRecoInit_ == nullptr)
	{
		funSpeechRecoInit_ = (LPEx_SpeechRecoInit)GetProcAddress(m_hSpeechTech, "Ex_SpeechRecoInit");
	}

	if (funSpeechRecoInit_)
		return funSpeechRecoInit_(CbResult, CbVolume, bInlineBtAcount, app_id, api_key, secret_key, dev_pid);

	return nullptr;
}

void CSpeechTechControl::SpeechRecoUnInit(void* pEngine)
{
	if (m_hSpeechTech == nullptr)
		return;

	if (funSpeechRecoUnInit_ == nullptr)
	{
		funSpeechRecoUnInit_ = (LPEx_SpeechRecoUnInit)GetProcAddress(m_hSpeechTech, "Ex_SpeechRecoUnInit");
	}

	if (funSpeechRecoUnInit_)
		return funSpeechRecoUnInit_(pEngine);
}

bool CSpeechTechControl::SpeechRecoStart(void* pEngine)
{
	if (m_hSpeechTech == nullptr)
		return false;

	if (funSpeechRecoStart_ == nullptr)
	{
		funSpeechRecoStart_ = (LPEx_SpeechRecoStart)GetProcAddress(m_hSpeechTech, "Ex_SpeechRecoStart");
	}

	if (funSpeechRecoStart_)
		return funSpeechRecoStart_(pEngine);

	return false;
}

bool CSpeechTechControl::SpeechRecoStop(void* pEngine)
{
	if (m_hSpeechTech == nullptr)
		return false;

	if (funSpeechRecoStop_ == nullptr)
	{
		funSpeechRecoStop_ = (LPEx_SpeechRecoStop)GetProcAddress(m_hSpeechTech, "Ex_SpeechRecoStop");
	}

	if (funSpeechRecoStop_)
		return funSpeechRecoStop_(pEngine);

	return false;
}
