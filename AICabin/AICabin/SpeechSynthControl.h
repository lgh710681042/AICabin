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
	void		SpeechSynthStartEnter(wstring& strText);

	//离开语音合成，回掉直接播放
	void		SpeechSynthStartLeave(wstring& strText);
};

