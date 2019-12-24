#include "stdafx.h"
#include <shlobj.h>


namespace CommonUtil
{
	wstring GetLocalPath()
	{
		TCHAR szPath[MAX_PATH * 2 + 1];
		if (!GetModuleFileName(NULL, szPath, _countof(szPath) - 1))
		{
			return _T("");
		}

		TCHAR* p = _tcsrchr(szPath, _T('\\'));
		if (p)
		{
			*p = _T('\0');
		}

		return szPath;
	}

	//wstring Utf8ToUnicode(const string& strUtf8)
	//{
	//	return Utf8ToUnicode(strUtf8.c_str());
	//}

	wstring Utf8ToUnicode(const char* pszUtf8)
	{
		if (!pszUtf8)
			return L"";

		int utf8_len = strlen(pszUtf8);
		int charcount = ::MultiByteToWideChar(CP_UTF8, 0, pszUtf8, utf8_len, NULL, 0);
		if (charcount == 0)
			return L"";

		wstring wide;
		wide.resize(charcount);
		::MultiByteToWideChar(CP_UTF8, 0, pszUtf8, utf8_len, &wide[0], charcount);

		return wide;
	}

	//string UnicodeToUtf8(const wstring& strUnicode)
	//{
	//	return UnicodeToUtf8(strUnicode.c_str());
	//}

	string UnicodeToUtf8(const wchar_t* pszUnicode)
	{
		if (!pszUnicode)
			return "";

		int unicode_len = wcslen(pszUnicode);
		int charcount = ::WideCharToMultiByte(CP_UTF8, 0, pszUnicode, unicode_len, NULL, 0, NULL, NULL);
		if (charcount == 0)
			return "";

		string mb;
		mb.resize(charcount);
		::WideCharToMultiByte(CP_UTF8, 0, pszUnicode, unicode_len, &mb[0], charcount, NULL, NULL);

		return mb;
	}

	std::wstring GetSystemAppDataFolder()
	{
		TCHAR wszAppDataPath[MAX_PATH * 2] = { 0 };
		SHGetSpecialFolderPath(NULL, wszAppDataPath, CSIDL_APPDATA, TRUE);
		return wszAppDataPath;
	}

	bool IsDirExist(const wstring& path)
	{
		DWORD dwAttri = ::GetFileAttributes(path.c_str());
		if ((dwAttri != -1) && (dwAttri&FILE_ATTRIBUTE_DIRECTORY))
			return true;

		return false;
	}

	vector<wstring> SplitW(const wstring& src, const wstring& pattern)
	{
		vector<wstring> result;
		if (src.empty())
			return result;

		wstring str = src + pattern;//扩展字符串以方便操作
		size_t size = str.size();
		wstring::size_type pos;

		for (size_t i = 0; i < size; i++)
		{
			pos = str.find(pattern, i);
			if (pos < size && pos != string::npos)
			{
				wstring s = str.substr(i, pos - i);
				result.push_back(s);
				i = pos + pattern.size() - 1;
			}
		}

		return result;
	}

	bool CreateDirectories(const wstring& path)
	{
		vector<wstring> direct = SplitW(path, L"\\");

		wstring cur_path;
		for (vector<wstring>::iterator iter = direct.begin(); iter != direct.end(); ++iter)
		{
			cur_path += *iter;
			cur_path += L"\\";
			if (_waccess(cur_path.c_str(), 0) != 0)
			{
				_wmkdir(cur_path.c_str());
			}
		}

		return IsDirExist(path);
	}

	std::wstring GetLogDir()
	{
		wstring strLogDir = GetSystemAppDataFolder() + _T("\\AICabin");

		if (!IsDirExist(strLogDir))
		{
			CreateDirectories(strLogDir);
		}

		return strLogDir;
	}

	std::wstring GetEnterMp3Path()
	{
		wstring strDir = GetSystemAppDataFolder() + _T("\\AICabin");

		if (!IsDirExist(strDir))
		{
			CreateDirectories(strDir);
		}

		return strDir + _T("\\enter.mp3");
	}

	std::wstring GetLeaveMp3Path()
	{
		wstring strDir = GetSystemAppDataFolder() + _T("\\AICabin");

		if (!IsDirExist(strDir))
		{
			CreateDirectories(strDir);
		}

		return strDir + _T("\\leave.mp3");
	}

	int ToolRandInt(int min, int max)
	{
		srand((unsigned)time(NULL));
		int r = rand() % (max - min + 1) + min;

		return r;
	}

}