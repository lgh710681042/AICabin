#include "stdafx.h"
#include "Singleton.h"
#include "CommonUtil.h"
#include "AICabinWnd.h"
#include "MainWnd.h"
#include "Application.h"
#include "SpeechSynthControl.h"


HINSTANCE CMainWnd::m_hInstance = NULL;
CMainWnd::CMainWnd()
{
	m_hWnd = NULL;
}


CMainWnd::~CMainWnd()
{
}

bool CMainWnd::RegisterWnd()
{
	if (m_hInstance)
	{
		return true;
	}

	WNDCLASSEX wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = ::GetModuleHandle(NULL);
	wcex.hIcon = LoadIcon(wcex.hInstance, IDI_APPLICATION);
	wcex.hCursor = LoadCursor(NULL, MAKEINTRESOURCE(IDC_ARROW));
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = _T("");
	wcex.lpszClassName = GetWindowClass();
	wcex.hIconSm = LoadIcon(wcex.hInstance, IDI_APPLICATION);

	bool bRes = RegisterClassEx(&wcex) != 0;
	if (bRes)
	{
		m_hInstance = wcex.hInstance;
	}

	return bRes;
}

HWND CMainWnd::CreateWnd()
{
	m_hWnd = CreateWindow(GetWindowClass(), _T("ND_PRESENTER_MSG_WND"), WS_OVERLAPPED, 0, 0, 0, 0, HWND_MESSAGE, NULL, m_hInstance, this);

	return m_hWnd;
}

void CMainWnd::LoopMessage()
{
	MSG msg = { 0 };
	while (::IsWindow(m_hWnd)
		&& ::GetMessage(&msg, NULL, 0, 0))
	{
		::TranslateMessage(&msg);
		::DispatchMessage(&msg);

		if (msg.message == WM_QUIT) break;
	}
}

LPCTSTR CMainWnd::GetWindowClass()
{
	return _T("ND_PRESENTER_MSG_WND");
}

LRESULT CALLBACK CMainWnd::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	CMainWnd* pThis = NULL;
	if (uMsg == WM_NCCREATE && lParam != NULL) {
		LPCREATESTRUCT lpcs = reinterpret_cast<LPCREATESTRUCT>(lParam);
		pThis = static_cast<CMainWnd*>(lpcs->lpCreateParams);
		::SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LPARAM>(pThis));
	}
	else
	{
		pThis = reinterpret_cast<CMainWnd*>(::GetWindowLongPtr(hWnd, GWLP_USERDATA));

		if (uMsg == WM_NCDESTROY) {
			pThis = NULL;
			::SetWindowLongPtr(hWnd, GWLP_USERDATA, NULL);
		}
	}

	if (pThis)
	{
		pThis->ThisWndProc(hWnd, uMsg, wParam, lParam);
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

LRESULT CMainWnd::ThisWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	// 消息处理
	switch (uMsg)
	{
	case WM_COPYDATA:
		HandCopyData(hWnd, uMsg, wParam, lParam);
		break;
	case WM_AICABIN_OPEN:
		HandAICabinOpen(hWnd, uMsg, wParam, lParam);
		break;
	default:
		break;
	}

	return FALSE;
}



void CMainWnd::HandCopyData(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	COPYDATASTRUCT* pCopyDataStruct = (COPYDATASTRUCT*)lParam;
}

void CMainWnd::HandAICabinOpen(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (m_pAICabinWnd == nullptr)
	{
		m_pAICabinWnd = new CAICabinWnd;
		if (m_pAICabinWnd == nullptr)
			return;

		if (!m_pAICabinWnd->CreateWnd())
			return;

		m_pAICabinWnd->SetAutoDel(true);
	}

	if (m_pAICabinWnd == nullptr)
		return;

	wstring strTipsStart = m_pAICabinWnd->SetTipsStart(CApplication::GetInstance()->GetUserName());
	//语音合成播放
	CSpeechSynthControl::GetInstance()->ControlSpeechSynthStartEnter(strTipsStart);

	m_pAICabinWnd->ShowWindow();
	m_pAICabinWnd->SetFullScreen(true);
}

