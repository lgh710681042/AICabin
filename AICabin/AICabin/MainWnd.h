#pragma once

class CMainWnd
{
public:
	CMainWnd();
	~CMainWnd();

public:
	static bool				RegisterWnd();

	HWND					CreateWnd();

	void					LoopMessage();

protected:
	static LPCTSTR			GetWindowClass();

	static LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	LRESULT					ThisWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

private:

	void					HandCopyData(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	void					HandAICabinOpen(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
	static HINSTANCE		m_hInstance;
	HWND					m_hWnd;

	CAICabinWnd*			m_pAICabinWnd = nullptr;
};

