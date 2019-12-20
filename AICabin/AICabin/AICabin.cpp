// AICabin.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "Singleton.h"
#include "MainWnd.h"
#include "Application.h"
#include "AICabin.h"

#define MAX_LOADSTRING 100

std::shared_ptr<CApplication> s_spApp = nullptr;

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO:  在此放置代码。
	
	s_spApp = CApplication::GetInstance();
	if (s_spApp == nullptr)
		return 0;

	if (s_spApp->Initialize(hInstance, lpCmdLine) == false)
		return 0;

	if (s_spApp->Run() == false)
		return 0;

	return 0;
}

