#pragma once

//语音合成与播放控制单例

class CSpeechSynthControl
	: public Util::Singleton<CSpeechSynthControl>
{
public:
	CSpeechSynthControl();
	~CSpeechSynthControl();

	bool		Init();

	//入舱语音合成，回掉直接播放
	void		ControlSpeechSynthStartEnter(wstring& strText);

	//离开语音合成，回掉直接播放
	void		ControlSpeechSynthStartLeave(wstring& strText);

    //呼叫语音合成，回掉直接播放
    void        ControlSpeechSynthCall(wstring& strText);

	//人脸表情识别界面，合成语音，回掉播放
	void		ControlSpeechSynthCallFace(wstring& strText, HWND hFaceWnd);

	HWND		GetFaceWnd(){ return m_hFaceWnd; }

private:
	HWND		m_hFaceWnd;
};

