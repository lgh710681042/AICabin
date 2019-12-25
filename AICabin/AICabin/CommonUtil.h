#pragma once

#define WM_AICABIN_OPEN		WM_USER + 101
#define WM_SPEAK_RESULT		WM_USER + 102
#define WM_FACE_HBITMAP		WM_USER + 103

typedef struct AutoLock
{
	AutoLock(CRITICAL_SECTION* lck)
	{
		_lck = lck;
		::EnterCriticalSection(_lck);
	}

	~AutoLock()
	{
		::LeaveCriticalSection(_lck);
	}

	CRITICAL_SECTION* _lck;

}AutoLock;

namespace CommonUtil
{
	wstring
		GetLocalPath();

	/*wstring
		Utf8ToUnicode(const string& strUtf8);*/

	wstring
		Utf8ToUnicode(const char* pszUtf8);

	/*string
		UnicodeToUtf8(const wstring& strUnicode);*/

	string
		UnicodeToUtf8(const wchar_t* pszUnicode);

	wstring
		GetSystemAppDataFolder();

	bool
		IsDirExist(const wstring& path);

	vector<wstring>
		SplitW(const wstring& src, const wstring& pattern);

	bool
		CreateDirectories(const wstring& path);

	wstring
		GetLogDir();

	wstring
		GetEnterMp3Path();

	wstring
		GetLeaveMp3Path();

	int
		ToolRandInt(int min, int max);
}