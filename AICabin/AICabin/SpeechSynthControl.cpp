#include "stdafx.h"
#include "Singleton.h"
#include "CommonUtil.h"
#include "SpeechTechControl.h"
#include "SpeechSynthControl.h"

void __stdcall onSynthResultEnter(int nCode, char *strMsg)
{
	if (nCode == 0)
	{
		wstring strMp3 = CommonUtil::GetEnterMp3Path();
		WCHAR szShortPath[256] = { 0 };
		WCHAR szCommand[256] = { 0 };
		GetShortPathName(strMp3.c_str(), szShortPath, sizeof(szShortPath));

		wsprintf(szCommand, L"open \"%s\" alias song", szShortPath);
		mciSendString(szCommand, nullptr, 0, nullptr);
		mciSendString(L"play song", nullptr, 0, nullptr);
	}
}

void __stdcall onSynthResultLeave(int nCode, char *strMsg)
{
	if (nCode == 0)
	{
		wstring strMp3 = CommonUtil::GetLeaveMp3Path();
		WCHAR szShortPath[256] = { 0 };
		WCHAR szCommand[256] = { 0 };
		GetShortPathName(strMp3.c_str(), szShortPath, sizeof(szShortPath));

		wsprintf(szCommand, L"open \"%s\" alias song2", szShortPath);
		mciSendString(szCommand, nullptr, 0, nullptr);
		mciSendString(L"play song2", nullptr, 0, nullptr);
	}
}

CSpeechSynthControl::CSpeechSynthControl()
{
	Init();
}


CSpeechSynthControl::~CSpeechSynthControl()
{
	//暂时不释放，会奔溃
	//CSpeechTechControl::GetInstance()->SpeechSynthUnInit();
}

bool CSpeechSynthControl::Init()
{
	return CSpeechTechControl::GetInstance()->SpeechSynthInit();
}

void CSpeechSynthControl::ControlSpeechSynthStartEnter(wstring& strText)
{
	bool bResult = CSpeechTechControl::GetInstance()->SpeechSynthStartAsyn(CommonUtil::UnicodeToUtf8(strText.c_str()).c_str(),
		CommonUtil::UnicodeToUtf8(CommonUtil::GetEnterMp3Path().c_str()).c_str(),
		"4",
		onSynthResultEnter);
}

void CSpeechSynthControl::ControlSpeechSynthStartLeave(wstring& strText)
{
	bool bResult = CSpeechTechControl::GetInstance()->SpeechSynthStartAsyn(CommonUtil::UnicodeToUtf8(strText.c_str()).c_str(),
		CommonUtil::UnicodeToUtf8(CommonUtil::GetLeaveMp3Path().c_str()).c_str(),
		"4",
		onSynthResultLeave);
}



