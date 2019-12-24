#include "stdafx.h"
#include "Singleton.h"
#include "CommonUtil.h"
#include "AICabinWnd.h"
#include "MainWnd.h"
#include "NDHttp/NDHttp.h"
#include "NDHttp/NDHttpBiz.h"
#include "NDHttp_Wrapper.hpp"
#include "Application.h"
#include "NewUI/UIAutoSizeButton.h"


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
	// ¿ª²Õ½Ó¿Ú²âÊÔ
	/*CHttpRequestDataModel Model;
	Model.strHost = "ai-learning-box-service.beta.101.com";
	Model.strUrl = "/v0.1/visitor/opening_infos?cabin_code=ai_learning_box";
	Model.strMethod = "PUT";
	Model.strHeader = "Accept: application/json\r\nContent-Type: application/json\r\nsdp-app-id:44cebc7f-7b35-4e4d-95d0-3c7af9db955e";
	Model.nPort = 443;

	string strData;
	DWORD code = NDhttp_Wrapper::Excute(&Model, strData);
	wstring strUnicode = CommonUtil::Utf8ToUnicode(strData.c_str());*/

	m_pMainWnd = new CMainWnd();
	if (m_pMainWnd)
	{
		m_hMsgWnd = m_pMainWnd->CreateWnd();
	}

	CAICabinWnd* pAICabinWnd = new CAICabinWnd;
	if (!pAICabinWnd)
		return false;

	if (!pAICabinWnd->CreateWnd())
		return false;

	pAICabinWnd->SetAutoDel(true);

	//pAICabinWnd->CenterWindow();
	pAICabinWnd->ShowWindow();

	pAICabinWnd->SetFullScreen(true);

	//message loop
	m_pMainWnd->LoopMessage();

	return true;
}

HWND CApplication::GetMainHwnd()
{
	return m_hMsgWnd;
}

bool CApplication::InitDirectUI(HINSTANCE hInstance)
{
	GetUIEngine()->InitSkin();
	GetUIRes()->SetDefaultImageType(UITYPE_IMAGE::UIIAMGE_BITMAP);
    GetUIRes()->SetLanguage(_T("zh_CN"));

	tstring strLocalPath = CommonUtil::GetLocalPath();

	tstring strFrameworkPath = strLocalPath + _T("\\framework\\");
	GetUIRes()->InitResDir(strFrameworkPath.c_str());

	tstring strSkinsPath = strLocalPath + _T("\\Skins\\");
	GetUIRes()->InitResDir(strSkinsPath.c_str(), true);

    UI_OBJECT_REGISTER(CAutoSizeButtonUI);

	return true;
}
