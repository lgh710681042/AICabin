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
	// 暂时不需要
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
	人脸表情识别
	\n nTimes 识别多少秒，默认5s
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
		return prewidth;//宽度 pix
	}
	else
	{
		return (prewidth / 4 + 1) * 4;//宽度 pix
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
	DWORD biSizeImage; // 位图的大小，以字节为单位
	LONG biXPelsPerMeter; // 位图水平分辨率，每米像素数
	LONG biYPelsPerMeter; // 位图垂直分辨率，每米像素数
	DWORD biClrUsed;// 位图实际使用的颜色表中的颜色数
	DWORD biClrImportant;// 位图显示过程中重要的颜色数72*8
	*/
	char *pBits = NULL;
	HBITMAP hBitmap = CreateDIBSection(hDC, bmi, DIB_RGB_COLORS, (void**)&pBits, NULL, 0);
	int pixelBytes = imgTmp.channels()*(imgTmp.depth() + 1); // 计算一个像素多少个字节

	if (bmp_w % 4 == 0)
	{
		memcpy(pBits, imgTmp.data, pixelBytes*bmp_w*bmp_h);
	}
	else
	{
		// 原始图像宽度不是 4 的倍数，将实际图片数据一行一行拷贝过去 
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
	bmih->biSize = sizeof(BITMAPINFOHEADER);//指定文件大小，包括这14个字节
	/*32位的Windows操作系统处理4个字节(32位)的速度比较快，所以BMP的每一行颜色占用的字节数规定为4的整数倍。MyBmp.bmp中一行颜色有两个像素，共占用6字节，如果要补齐4*2=8字节，就要再加两个0字节。
	行补位的公式为：widthBytes = (width*biBitCount+31)/32*4 */
	//width = (width*bpp + width) / 32 * 4;
	bmih->biWidth = GetSuiteableWidth(width);//宽度 pix
	bmih->biHeight = origin ? abs(height) : -abs(height); // 高度  .bMP文件的数据从下到上，从左到右的。也就是说，从文件中最先读到的是图象最下面一行的左边第一个象素，然后是左边第二个象素……接下来是倒数第二行左边第一个象素，左边第二个象素……依次类推 ，最后得到的是最上面一行的最右一个象素。
	bmih->biPlanes = 1; // 目标设备的级别，必须为1
	bmih->biBitCount = (unsigned short)bpp;// 每个像素所需的位数，必须是1(双色), 每个像素的位数
	// 1 - 黑白图，4 - 16色，8 - 256色，24 - 真彩色 4(16色)，8(256色)或24(真彩色)之一
	bmih->biCompression = BI_RGB;// 位图压缩类型，必须是 0(不压缩), 1(BI_RLE8压缩类型)或2(BI_RLE4压缩类型)之一
	//	bmih->biSizeImage = bmih->biHeight * 8 *height;
	/*biSizeImage
	指定实际的位图数据占用的字节数，其实也可以从以下的公式中计算出来：
	biSizeImage=biWidth’ × biHeight
	要注意的是：上述公式中的biWidth’必须是4的整倍数(所以不是biWidth，而是biWidth’，
	表示大于或等于biWidth的，最接近4的整倍数。举个例子，如果biWidth=240，则biWidth’=240；
	如果biWidth=241，biWidth’=244)。*/
}

void COpencvControl::SaveBitmapToFile(HBITMAP hBitmap, TCHAR* szfilename)
{
	HDC hdc;			//设备描述表
	int ibits;
	WORD wbitcount;     //当前显示分辨率下每个像素所占字节数

	//位图中每个像素所占字节数，定义调色板大小，位图中像素字节大小，位图文件大小 ，写入文件字节数
	DWORD dwpalettesize = 0, dwbmbitssize, dwdibsize, dwwritten;

	BITMAP bitmap;				//位图属性结构
	BITMAPFILEHEADER bmfhdr;	//位图文件头结构
	BITMAPINFOHEADER bi;		//位图信息头结构
	LPBITMAPINFOHEADER lpbi;	//指向位图信息头结构

	//定义文件，分配内存句柄，调色板句柄
	HANDLE fh, hdib, hpal, holdpal = NULL;

	//计算位图文件每个像素所占字节数
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

	//计算调色板大小
	if (wbitcount <= 8)
		dwpalettesize = (1 << wbitcount) * sizeof(RGBQUAD);

	//设置位图信息头结构
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
	//为位图内容分配内存
	hdib = GlobalAlloc(GHND, dwbmbitssize + dwpalettesize + sizeof(BITMAPINFOHEADER));
	lpbi = (LPBITMAPINFOHEADER)GlobalLock(hdib);
	*lpbi = bi;

	// 处理调色板 
	hpal = GetStockObject(DEFAULT_PALETTE);
	if (hpal)
	{
		hdc = ::GetDC(NULL);
		holdpal = SelectPalette(hdc, (HPALETTE)hpal, false);
		RealizePalette(hdc);
	}

	// 获取该调色板下新的像素值
	GetDIBits(hdc, hBitmap, 0, (UINT)bitmap.bmHeight, (LPSTR)lpbi + sizeof(BITMAPINFOHEADER) + dwpalettesize, (BITMAPINFO*)lpbi, DIB_RGB_COLORS);

	//恢复调色板 
	if (holdpal)
	{
		SelectPalette(hdc, (HPALETTE)holdpal, true);
		RealizePalette(hdc);
		::ReleaseDC(NULL, hdc);
	}

	//创建位图文件 
	fh = CreateFile(szfilename, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN, NULL);
	if (fh == INVALID_HANDLE_VALUE)
	{
		DWORD dw = GetLastError();
		return;
	}

	// 设置位图文件头
	bmfhdr.bfType = 0x4d42; // "bm"
	dwdibsize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) +
		dwpalettesize + dwbmbitssize;
	bmfhdr.bfSize = dwdibsize;
	bmfhdr.bfReserved1 = 0;
	bmfhdr.bfReserved2 = 0;
	bmfhdr.bfOffBits = (DWORD)sizeof(BITMAPFILEHEADER) +
		(DWORD)sizeof(BITMAPINFOHEADER) + dwpalettesize;

	//写入位图文件头
	WriteFile(fh, (LPSTR)&bmfhdr, sizeof(BITMAPFILEHEADER), &dwwritten, NULL);

	//写入位图文件其余内容
	WriteFile(fh, (LPSTR)lpbi, dwdibsize, &dwwritten, NULL);
	//清除 
	GlobalUnlock(hdib);
	GlobalFree(hdib);
	CloseHandle(fh);
}
