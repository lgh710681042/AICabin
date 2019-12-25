#include "stdafx.h"
#include "Singleton.h"
#include "CommonUtil.h"
#include "qFaceExpression.h"
#include "OpencvControl.h"


COpencvControl::COpencvControl()
{
	Init();

	InitializeCriticalSection(&m_ControlLock);
}


COpencvControl::~COpencvControl()
{
	DeleteCriticalSection(&m_ControlLock);
}

void COpencvControl::Init()
{
	// ��ʱ����Ҫ
}

bool COpencvControl::BeginFace(HWND hMsgWnd)
{
	AutoLock lock(&m_ControlLock);

	m_hMsgWnd = hMsgWnd;

	if (m_pFaceExpress == nullptr)
	{
		m_pFaceExpress = new FaceExpression;
	}

	if (m_pFaceExpress)
	{
		m_pFaceExpress->init("");
		cv::namedWindow("face_detect", cv::WINDOW_AUTOSIZE);

		std::thread t([this]{
			AutoLock lock(&m_ControlLock);
			cv::Mat img_decode;
			
			do 
			{
				::Sleep(100);
				img_decode = m_pFaceExpress->getImg();
				if (img_decode.empty())
					continue;

				HBITMAP hBitMap = MatImage2CBitmap(::GetDC(NULL), img_decode);

				SaveBitmapToFile(hBitMap, L"F:\\1.bmp");

				cv::imshow("face_detect", img_decode);

				OutputDebugStringA(m_pFaceExpress->getExpression().c_str());

			} while (true);
		});

		t.detach();
	}

	return true;
}

/*!
	��������ʶ��
	\n nTimes ʶ������룬Ĭ��5s
*/
bool COpencvControl::BeginFaceExpRecognition(int nTimes)
{
	AutoLock lock(&m_ControlLock);

	return true;
}

bool COpencvControl::EndFace()
{
	AutoLock lock(&m_ControlLock);

	m_hMsgWnd = nullptr;

	if (m_pFaceExpress)
	{
		m_pFaceExpress->clean();

		delete m_pFaceExpress;
		m_pFaceExpress = nullptr;
	}

	return true;
}

int GetSuiteableWidth(const int prewidth)
{
	int nwidth = 0;
	int nremainder = prewidth % 4;
	if (nremainder == 0)
	{
		return prewidth;//��� pix
	}
	else
	{
		return (prewidth / 4 + 1) * 4;//��� pix
	}
	return 0;
}

HBITMAP COpencvControl::MatImage2CBitmap(HDC hDC, const cv::Mat imgTmp)
{
	uchar buffer[sizeof(BITMAPINFOHEADER) + 1024];
	BITMAPINFO* bmi = (BITMAPINFO*)buffer;
	int bmp_w = imgTmp.cols, bmp_h = imgTmp.rows;
	int nchannels = imgTmp.channels();
	int bpp = (imgTmp.depth() + 1)*nchannels;
	FillBitmapInfo(bmi, bmp_w, bmp_h, bpp * 8, 0);
	/*
	DWORD biSizeImage; // λͼ�Ĵ�С�����ֽ�Ϊ��λ
	LONG biXPelsPerMeter; // λͼˮƽ�ֱ��ʣ�ÿ��������
	LONG biYPelsPerMeter; // λͼ��ֱ�ֱ��ʣ�ÿ��������
	DWORD biClrUsed;// λͼʵ��ʹ�õ���ɫ���е���ɫ��
	DWORD biClrImportant;// λͼ��ʾ��������Ҫ����ɫ��72*8
	*/
	char *pBits = NULL;
	HBITMAP hBitmap = CreateDIBSection(hDC, bmi, DIB_RGB_COLORS, (void**)&pBits, NULL, 0);
	int pixelBytes = imgTmp.channels()*(imgTmp.depth() + 1); // ����һ�����ض��ٸ��ֽ�

	if (bmp_w % 4 == 0)
	{
		memcpy(pBits, imgTmp.data, pixelBytes*bmp_w*bmp_h);
	}
	else
	{
		// ԭʼͼ���Ȳ��� 4 �ı�������ʵ��ͼƬ����һ��һ�п�����ȥ 
		int ntempW = GetSuiteableWidth(bmp_w);
		for (size_t i = 0; i < bmp_h; i++)
		{
			memcpy(pBits + pixelBytes * ntempW * i, imgTmp.data + pixelBytes * bmp_w * i, pixelBytes*bmp_w);
		}
	}

	return hBitmap;
}

void COpencvControl::FillBitmapInfo(BITMAPINFO* bmi, int width, int height, int bpp, int origin)
{
	BITMAPINFOHEADER* bmih = &(bmi->bmiHeader);

	memset(bmih, 0, sizeof(*bmih));
	bmih->biSize = sizeof(BITMAPINFOHEADER);//ָ���ļ���С��������14���ֽ�
	/*32λ��Windows����ϵͳ����4���ֽ�(32λ)���ٶȱȽϿ죬����BMP��ÿһ����ɫռ�õ��ֽ����涨Ϊ4����������MyBmp.bmp��һ����ɫ���������أ���ռ��6�ֽڣ����Ҫ����4*2=8�ֽڣ���Ҫ�ټ�����0�ֽڡ�
	�в�λ�Ĺ�ʽΪ��widthBytes = (width*biBitCount+31)/32*4 */
	//width = (width*bpp + width) / 32 * 4;
	bmih->biWidth = GetSuiteableWidth(width);//��� pix
	bmih->biHeight = origin ? abs(height) : -abs(height); // �߶�  .bMP�ļ������ݴ��µ��ϣ������ҵġ�Ҳ����˵�����ļ������ȶ�������ͼ��������һ�е���ߵ�һ�����أ�Ȼ������ߵڶ������ء����������ǵ����ڶ�����ߵ�һ�����أ���ߵڶ������ء����������� �����õ�����������һ�е�����һ�����ء�
	bmih->biPlanes = 1; // Ŀ���豸�ļ��𣬱���Ϊ1
	bmih->biBitCount = (unsigned short)bpp;// ÿ�����������λ����������1(˫ɫ), ÿ�����ص�λ��
	// 1 - �ڰ�ͼ��4 - 16ɫ��8 - 256ɫ��24 - ���ɫ 4(16ɫ)��8(256ɫ)��24(���ɫ)֮һ
	bmih->biCompression = BI_RGB;// λͼѹ�����ͣ������� 0(��ѹ��), 1(BI_RLE8ѹ������)��2(BI_RLE4ѹ������)֮һ
	//	bmih->biSizeImage = bmih->biHeight * 8 *height;
	/*biSizeImage
	ָ��ʵ�ʵ�λͼ����ռ�õ��ֽ�������ʵҲ���Դ����µĹ�ʽ�м��������
	biSizeImage=biWidth�� �� biHeight
	Ҫע����ǣ�������ʽ�е�biWidth��������4��������(���Բ���biWidth������biWidth����
	��ʾ���ڻ����biWidth�ģ���ӽ�4�����������ٸ����ӣ����biWidth=240����biWidth��=240��
	���biWidth=241��biWidth��=244)��*/
}

void COpencvControl::SaveBitmapToFile(HBITMAP hBitmap, TCHAR* szfilename)
{
	HDC hdc;			//�豸������
	int ibits;
	WORD wbitcount;     //��ǰ��ʾ�ֱ�����ÿ��������ռ�ֽ���

	//λͼ��ÿ��������ռ�ֽ����������ɫ���С��λͼ�������ֽڴ�С��λͼ�ļ���С ��д���ļ��ֽ���
	DWORD dwpalettesize = 0, dwbmbitssize, dwdibsize, dwwritten;

	BITMAP bitmap;				//λͼ���Խṹ
	BITMAPFILEHEADER bmfhdr;	//λͼ�ļ�ͷ�ṹ
	BITMAPINFOHEADER bi;		//λͼ��Ϣͷ�ṹ
	LPBITMAPINFOHEADER lpbi;	//ָ��λͼ��Ϣͷ�ṹ

	//�����ļ��������ڴ�������ɫ����
	HANDLE fh, hdib, hpal, holdpal = NULL;

	//����λͼ�ļ�ÿ��������ռ�ֽ���
	hdc = CreateDC(L"display", NULL, NULL, NULL);
	ibits = GetDeviceCaps(hdc, BITSPIXEL) * GetDeviceCaps(hdc, PLANES);
	DeleteDC(hdc);

	if (ibits <= 1)
		wbitcount = 1;
	else if (ibits <= 4)
		wbitcount = 4;
	else if (ibits <= 8)
		wbitcount = 8;
	else if (ibits <= 16)
		wbitcount = 16;
	else if (ibits <= 24)
		wbitcount = 24;
	else
		wbitcount = 32;

	//�����ɫ���С
	if (wbitcount <= 8)
		dwpalettesize = (1 << wbitcount) * sizeof(RGBQUAD);

	//����λͼ��Ϣͷ�ṹ
	GetObject(hBitmap, sizeof(BITMAP), (LPSTR)&bitmap);
	bi.biSize = sizeof(BITMAPINFOHEADER);
	bi.biWidth = bitmap.bmWidth;
	bi.biHeight = bitmap.bmHeight;
	bi.biPlanes = 1;
	bi.biBitCount = wbitcount;
	bi.biCompression = BI_RGB;
	bi.biSizeImage = 0;
	bi.biXPelsPerMeter = 0;
	bi.biYPelsPerMeter = 0;
	bi.biClrUsed = 0;
	bi.biClrImportant = 0;

	dwbmbitssize = ((bitmap.bmWidth * wbitcount + 31) / 32) * 4 * bitmap.bmHeight;
	//Ϊλͼ���ݷ����ڴ�
	hdib = GlobalAlloc(GHND, dwbmbitssize + dwpalettesize + sizeof(BITMAPINFOHEADER));
	lpbi = (LPBITMAPINFOHEADER)GlobalLock(hdib);
	*lpbi = bi;

	// �����ɫ�� 
	hpal = GetStockObject(DEFAULT_PALETTE);
	if (hpal)
	{
		hdc = ::GetDC(NULL);
		holdpal = SelectPalette(hdc, (HPALETTE)hpal, false);
		RealizePalette(hdc);
	}

	// ��ȡ�õ�ɫ�����µ�����ֵ
	GetDIBits(hdc, hBitmap, 0, (UINT)bitmap.bmHeight, (LPSTR)lpbi + sizeof(BITMAPINFOHEADER) + dwpalettesize, (BITMAPINFO*)lpbi, DIB_RGB_COLORS);

	//�ָ���ɫ�� 
	if (holdpal)
	{
		SelectPalette(hdc, (HPALETTE)holdpal, true);
		RealizePalette(hdc);
		::ReleaseDC(NULL, hdc);
	}

	//����λͼ�ļ� 
	fh = CreateFile(szfilename, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN, NULL);
	if (fh == INVALID_HANDLE_VALUE)
	{
		DWORD dw = GetLastError();
		return;
	}

	// ����λͼ�ļ�ͷ
	bmfhdr.bfType = 0x4d42; // "bm"
	dwdibsize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) +
		dwpalettesize + dwbmbitssize;
	bmfhdr.bfSize = dwdibsize;
	bmfhdr.bfReserved1 = 0;
	bmfhdr.bfReserved2 = 0;
	bmfhdr.bfOffBits = (DWORD)sizeof(BITMAPFILEHEADER) +
		(DWORD)sizeof(BITMAPINFOHEADER) + dwpalettesize;

	//д��λͼ�ļ�ͷ
	WriteFile(fh, (LPSTR)&bmfhdr, sizeof(BITMAPFILEHEADER), &dwwritten, NULL);

	//д��λͼ�ļ���������
	WriteFile(fh, (LPSTR)lpbi, dwdibsize, &dwwritten, NULL);
	//��� 
	GlobalUnlock(hdib);
	GlobalFree(hdib);
	CloseHandle(fh);
}
