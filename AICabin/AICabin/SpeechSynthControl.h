#pragma once

//�����ϳ��벥�ſ��Ƶ���

class CSpeechSynthControl
	: public Util::Singleton<CSpeechSynthControl>
{
public:
	CSpeechSynthControl();
	~CSpeechSynthControl();

	bool		Init();

	//��������ϳɣ��ص�ֱ�Ӳ���
	void		ControlSpeechSynthStartEnter(wstring& strText);

	//�뿪�����ϳɣ��ص�ֱ�Ӳ���
	void		ControlSpeechSynthStartLeave(wstring& strText);
};

