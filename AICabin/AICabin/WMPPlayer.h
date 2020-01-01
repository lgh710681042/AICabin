#pragma once
#include <wmp.h>

class CWMPPlayer;
class CWMPPlayerControl 
	: public Util::Singleton<CWMPPlayerControl>
{
public:
	CWMPPlayerControl();
	~CWMPPlayerControl();

	bool Play(wstring& strUrl);

	bool Stop();

private:
	shared_ptr<CWMPPlayer> m_spWMPlayer = nullptr;
};

class CWMPPlayer
{
public:
	CWMPPlayer();
	~CWMPPlayer();

public:
	bool PutUrl(wstring& strUrl);

	bool Play();

	bool Stop();

	bool Pause();

private:
	bool Init();

private:
	CComPtr<IWMPPlayer> m_spPlayer = nullptr;
	CComPtr<IWMPControls> m_spControls = nullptr;
};

