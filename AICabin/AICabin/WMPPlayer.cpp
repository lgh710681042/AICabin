#include "stdafx.h"
#include "Singleton.h"
#include "WMPPlayer.h"


CWMPPlayer::CWMPPlayer()
{
	CoInitialize(NULL);

	Init();
}


CWMPPlayer::~CWMPPlayer()
{
	CoUninitialize();
}

bool CWMPPlayer::Init()
{
	HRESULT hr = m_spPlayer.CoCreateInstance(__uuidof(WindowsMediaPlayer), 0, CLSCTX_INPROC_SERVER);

	if (SUCCEEDED(hr))
	{
		hr = m_spPlayer->get_controls(&m_spControls);
	}

	return SUCCEEDED(hr);
}

bool CWMPPlayer::PutUrl(wstring& strUrl)
{
	if (m_spPlayer == nullptr)
		return false;

	HRESULT hr = m_spPlayer->put_URL(_bstr_t(strUrl.c_str()));

	return SUCCEEDED(hr);
}

bool CWMPPlayer::Play()
{
	if (m_spControls == nullptr)
		return false;

	HRESULT hr = m_spControls->play();

	return SUCCEEDED(hr);
}

bool CWMPPlayer::Stop()
{
	if (m_spControls == nullptr)
		return false;

	HRESULT hr = m_spControls->stop();

	return SUCCEEDED(hr);
}

bool CWMPPlayer::Pause()
{
	if (m_spControls == nullptr)
		return false;

	HRESULT hr = m_spControls->pause();

	return SUCCEEDED(hr);
}

CWMPPlayerControl::CWMPPlayerControl()
{

}

CWMPPlayerControl::~CWMPPlayerControl()
{

}

bool CWMPPlayerControl::Play(wstring& strUrl)
{
	if (m_spWMPlayer == nullptr)
	{
		m_spWMPlayer = make_shared<CWMPPlayer>();
	}

	do 
	{
		if (m_spWMPlayer == nullptr)
			break;

		 
		if (m_spWMPlayer->Stop() == false)
			break;

		if(m_spWMPlayer->PutUrl(strUrl) == false)
			break;

		return m_spWMPlayer->Play();

	} while (false);

	return false;
	
}

bool CWMPPlayerControl::Stop()
{
	if (m_spWMPlayer == nullptr)
		return false;

	return m_spWMPlayer->Stop();
}
