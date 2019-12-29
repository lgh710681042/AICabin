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
	bool			BeginFaceExpRecognition();

	// 结束表情识别
	bool			EndFaceExpRecognition();

	// 关闭摄像头获取数据
	bool			EndFace();

	// 获取表情识别结果
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

	map<string, int>		m_mapExpress;			//表情识别结果
};

