#pragma once

class COpencvControl 
	: public Util::Singleton<COpencvControl>
{
public:
	COpencvControl();
	~COpencvControl();

	void			Init();

	// 开启摄像头获取数据
	bool			BeginFace(HWND hMsgWnd);

	// 开始表情识别
	bool			BeginFaceExpRecognition(int nTimes);

	// 关闭摄像头获取数据
	bool			EndFace();

	HBITMAP			MatImage2CBitmap(HDC hDC, const cv::Mat imgTmp);

	void			FillBitmapInfo(BITMAPINFO* bmi, int width, int height, int bpp, int origin);

	void			SaveBitmapToFile(HBITMAP hBitmap, TCHAR* szfilename);

private:
	FaceExpression* m_pFaceExpress = nullptr;
	HWND m_hMsgWnd = nullptr;

	CRITICAL_SECTION		m_ControlLock;
};

