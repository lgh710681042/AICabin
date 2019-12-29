#pragma once

class COpencvControl 
	: public Util::Singleton<COpencvControl>
{
public:
	COpencvControl();
	~COpencvControl();

	void			Init();

	// ��������ͷ��ȡ����
	bool			BeginFace(HWND hMsgWnd);

	// ��ʼ����ʶ��
	bool			BeginFaceExpRecognition();

	// ��������ʶ��
	bool			EndFaceExpRecognition();

	// �ر�����ͷ��ȡ����
	bool			EndFace();

	// ��ȡ����ʶ����
	map<string, int>	GetExpress();

	HBITMAP			MatImage2CBitmap(HDC hDC, const cv::Mat imgTmp);

	void			FillBitmapInfo(BITMAPINFO* bmi, int width, int height, int bpp, int origin);

	void			SaveBitmapToFile(HBITMAP hBitmap, TCHAR* szfilename);
	void			CopyBitsData(BYTE* bOut, const cv::Mat imgTmp);

public:
	cv::Mat					m_Imgdecode;

	vector<unsigned char>	m_vecFace;

private:
	FaceExpression* m_pFaceExpress = nullptr;
	HWND m_hMsgWnd = nullptr;

	CRITICAL_SECTION		m_ControlLock;
	bool					m_bEndFace = false;
	bool					m_bBeginFaceExpRecognition = false;

	map<string, int>		m_mapExpress;			//����ʶ����
};

