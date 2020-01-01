#include "stdafx.h"
#include "Singleton.h"
#include "CommonUtil.h"
#include "WMPPlayer.h"
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

		//bool b = CWMPPlayerControl::GetInstance()->Play(wstring(szShortPath));
		wsprintf(szCommand, L"open \"%s\" alias song", szShortPath);
		mciSendString(szCommand, nullptr, 0, nullptr);
		mciSendString(L"play song", nullptr, 0, nullptr);

		Sleep(5 * 1000);
		mciSendString(_T("close song"), NULL, 0, NULL);
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

		Sleep(7 * 1000);
		mciSendString(_T("close song2"), NULL, 0, NULL);
	}
}

void __stdcall onSynthResultCall(int nCode, char *strMsg)
{
    if (nCode == 0)
    {
        wstring strMp3 = CommonUtil::GetCallMp3Path();
        WCHAR szShortPath[256] = { 0 };
        WCHAR szCommand[256] = { 0 };
        GetShortPathName(strMp3.c_str(), szShortPath, sizeof(szShortPath));

        wsprintf(szCommand, L"open \"%s\" alias song3", szShortPath);
        mciSendString(szCommand, nullptr, 0, nullptr);

        WCHAR szPosition[256] = { 0 };//获取文件时长
        mciSendString(L"Status song3 length", szPosition, 255, 0);
        long lLength = wcstol(szPosition, NULL, 10);

        mciSendString(L"play song3", nullptr, 0, nullptr);

        Sleep(lLength + 500);
        mciSendString(_T("close song3"), NULL, 0, NULL);
    }
}

void __stdcall onSynthFaceResultCall(int nCode, char* strMsg)
{
	if (nCode == 0)
	{
		wstring strMp3 = CommonUtil::GetCallMp3Path();
		WCHAR szShortPath[256] = { 0 };
		WCHAR szCommand[256] = { 0 };
		GetShortPathName(strMp3.c_str(), szShortPath, sizeof(szShortPath));

		wsprintf(szCommand, L"open \"%s\" alias song3", szShortPath);
		mciSendString(szCommand, nullptr, 0, nullptr);

		WCHAR szPosition[256] = { 0 };//获取文件时长
		mciSendString(L"Status song3 length", szPosition, 255, 0);
		long lLength = wcstol(szPosition, NULL, 10);

		mciSendString(L"play song3", nullptr, 0, nullptr);

		Sleep(lLength + 500);
		mciSendString(_T("close song3"), NULL, 0, NULL);
	}

	::PostMessage(CSpeechSynthControl::GetInstance()->GetFaceWnd(), WM_FACE_AUDIO_CALL_END, NULL, NULL);
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

void CSpeechSynthControl::ControlSpeechSynthCall(wstring& strText)
{
    bool bResult = CSpeechTechControl::GetInstance()->SpeechSynthStartAsyn(CommonUtil::UnicodeToUtf8(strText.c_str()).c_str(),
        CommonUtil::UnicodeToUtf8(CommonUtil::GetCallMp3Path().c_str()).c_str(),
        "4",
        onSynthResultCall);
}

void CSpeechSynthControl::ControlSpeechSynthCallFace(wstring& strText, HWND hFaceWnd)
{
	m_hFaceWnd = hFaceWnd;

	bool bResult = CSpeechTechControl::GetInstance()->SpeechSynthStartAsyn(CommonUtil::UnicodeToUtf8(strText.c_str()).c_str(),
		CommonUtil::UnicodeToUtf8(CommonUtil::GetCallMp3Path().c_str()).c_str(),
		"4",
		onSynthFaceResultCall);
}
