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

    //���������ϳɣ��ص�ֱ�Ӳ���
    void        ControlSpeechSynthCall(wstring& strText);

	//��������ʶ����棬�ϳ��������ص�����
	void		ControlSpeechSynthCallFace(wstring& strText, HWND hFaceWnd);

	HWND		GetFaceWnd(){ return m_hFaceWnd; }

private:
	HWND		m_hFaceWnd;
};

