#pragma once

#include <string>
#include <windows.h>
#include <tchar.h>

using namespace std;

namespace CommonUtil
{


wstring GetLocalPath()
{
	TCHAR szPath[MAX_PATH * 2 + 1];
	if (!GetModuleFileName(NULL, szPath, _countof(szPath) - 1))
	{
		return _T("");
	}

	TCHAR* p = wcsrchr(szPath, _T('\\'));
	if (p)
	{
		*p = _T('\0');
	}

	return szPath;
}

}