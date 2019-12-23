#pragma once


namespace CommonUtil
{
	wstring
		GetLocalPath();

	wstring
		Utf8ToUnicode(const string& strUtf8);

	wstring
		Utf8ToUnicode(const char* pszUtf8);

	string
		UnicodeToUtf8(const wstring& strUnicode);

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
}