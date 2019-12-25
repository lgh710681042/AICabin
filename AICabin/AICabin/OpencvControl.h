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
	bool			BeginFaceExpRecognition(int nTimes);

	// �ر�����ͷ��ȡ����
	bool			EndFace();

	HBITMAP			MatImage2CBitmap(HDC hDC, const cv::Mat imgTmp);

	void			FillBitmapInfo(BITMAPINFO* bmi, int width, int height, int bpp, int origin);

	void			SaveBitmapToFile(HBITMAP hBitmap, TCHAR* szfilename);

private:
	FaceExpression* m_pFaceExpress = nullptr;
	HWND m_hMsgWnd = nullptr;

	CRITICAL_SECTION		m_ControlLock;
};

