#pragma once
#include "speechsynth/BtUtil/ResultDefine.h"
#include "speechsynth/SpeechRecoEx.h"
#include "speechsynth/SpeechSynthEx.h"

typedef bool(WINAPI *LPEx_SpeechSynthInit)(bool bInlineBtAcount, const char app_id[], const char api_key[], const char secret_key[]);
typedef bool(WINAPI *LPEx_SpeechSynthStartAsyn)(const char *strText, const char *strFilePath, const char *strper, onResultCallback CbResult);
typedef void(WINAPI *LPEx_SpeechSynthUnInit)();

typedef void*(WINAPI *LPEx_SpeechRecoInit)(onResultCallback CbResult,
	onVolumeCallback CbVolume,
	bool        bInlineBtAcount,
	const char *app_id,
	const char *api_key,
	const char *secret_key,
	DEV_PID dev_pid);
typedef void(WINAPI *LPEx_SpeechRecoUnInit)(void* pEngine);
typedef bool(WINAPI *LPEx_SpeechRecoStart)(void* pEngine);
typedef bool(WINAPI *LPEx_SpeechRecoStop)(void* pEngine);

class CSpeechTechControl 
	: public Util::Singleton<CSpeechTechControl>
{
public:
	CSpeechTechControl();
	~CSpeechTechControl();

	// 语音合成
	bool			InitModule();

	bool			SpeechSynthInit(bool bInlineBtAcount = true,
		const char app_id[] = "",
		const char api_key[] = "",
		const char secret_key[] = "");

	bool			SpeechSynthStartAsyn(const char *strText, 
		const char *strFilePath, 
		const char *strper = "0", 
		onResultCallback CbResult = NULL);

	void			SpeechSynthUnInit();

	//语音识别
	void*			SpeechRecoInit(onResultCallback CbResult,
		onVolumeCallback CbVolume,
		bool        bInlineBtAcount = true,
		const char *app_id = "",
		const char *api_key = "",
		const char *secret_key = "",
		DEV_PID dev_pid = CN);

	void			SpeechRecoUnInit(void* pEngine);

	bool			SpeechRecoStart(void* pEngine);

	bool			SpeechRecoStop(void* pEngine);

private:
	HMODULE m_hSpeechTech = nullptr;

	LPEx_SpeechSynthInit funSpeechSynthInit_ = nullptr;
	LPEx_SpeechSynthStartAsyn funSpeechSynthStartAsyn_ = nullptr;
	LPEx_SpeechSynthUnInit funSpeechSynthUnInit_ = nullptr;

	LPEx_SpeechRecoInit funSpeechRecoInit_ = nullptr;
	LPEx_SpeechRecoUnInit funSpeechRecoUnInit_ = nullptr;
	LPEx_SpeechRecoStart funSpeechRecoStart_ = nullptr;
	LPEx_SpeechRecoStop funSpeechRecoStop_ = nullptr;
};

