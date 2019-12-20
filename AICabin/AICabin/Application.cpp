#include "stdafx.h"
#include "Singleton.h"
#include "CommonUtil.h"
#include "MainWnd.h"
#include "Application.h"


CApplication::CApplication()
{
}


CApplication::~CApplication()
{
	if (m_pMainWnd)
	{
		delete m_pMainWnd;
		m_pMainWnd = nullptr;
	}
}

bool CApplication::Initialize(HINSTANCE hInstance, LPTSTR lpCmdLine)
{
	if (!InitDirectUI(hInstance))
		return false;

	CMainWnd::RegisterWnd();

	return true;
}

bool CApplication::Run()
{
	m_pMainWnd = new CMainWnd();
	if (m_pMainWnd)
	{
		m_hMsgWnd = m_pMainWnd->CreateWnd();
	}

	//message loop
	m_pMainWnd->LoopMessage();

	return true;
}

bool CApplication::InitDirectUI(HINSTANCE hInstance)
{
	GetUIEngine()->InitSkin();
	GetUIRes()->SetDefaultImageType(UITYPE_IMAGE::UIIAMGE_BITMAP);

	tstring strLocalPath = CommonUtil::GetLocalPath();

	tstring strFrameworkPath = strLocalPath + _T("\\framework\\");
	GetUIRes()->InitResDir(strFrameworkPath.c_str());

	tstring strSkinsPath = strLocalPath + _T("\\Skins\\");
	GetUIRes()->InitResDir(strSkinsPath.c_str(), true);

	return true;
}
