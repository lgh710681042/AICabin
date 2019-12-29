#pragma once
#include <opencv2/opencv.hpp>
#include <curl/curl.h>
#include <json/json.h>
#include <iostream>
#include <time.h>
#include "base64.h"
#include <vector>
#include <process.h>
#include <Kinect.h> //Kinect头文件
#include <time.h>
using namespace std;

#include "CommonUtil.h"
#include "AICabinWnd.h"
#include "MainWnd.h"
#include "Singleton.h"
#include "Application.h"

const int THREAD_NUM = 2;
typedef struct tagThreadManageMentDataKernel
{
	HANDLE hDetectThread;
	HANDLE hStartEvent;
	HANDLE hEndEvent;
	void * pParams;
	void * pDetectData;
	CvRect rcROI;
}ThreadManageMentDataKernel;

//线程管理模块，主要用于控制程序中线程的调度和启停
typedef struct tagThreadManageMentData
{
	ThreadManageMentDataKernel * m_pThreadManageMentDataKernel;
	int m_nKernelThreadCount;
	bool bStopThread;
	int nThreadIndex;
	bool bStart;
	std::string res_exp;
	std::string ref_exp;
	cv::VideoCapture vcap;
	Json::Value res_json;
	cv::Mat image;
	cv::Mat frame;
	cv::Rect box;

}ThreadManageMentData;

typedef struct tagDetectData
{
	ThreadManageMentData * m_pThreadManageMentData; //用于内存管理的内存
}DetectData;

// write date function
size_t write_data(void *ptr, size_t size, size_t nmemb, void *stream);

std::string img2base64(cv::Mat &img);



class FaceExpression
{
	std::string exp_str;
	DetectData * pDetectData;
	HANDLE handle[THREAD_NUM];

public:
	FaceExpression();
	~FaceExpression();

public:

	int init(std::string str_exp, int x, int y, int w, int h);

	void setExpression(std::string str_exp)
	{
		exp_str = str_exp;
	}

	std::string getImgBase64Encode()
	{
		if (!pDetectData->m_pThreadManageMentData->frame.empty())
		{
			return img2base64(pDetectData->m_pThreadManageMentData->frame);
		}
		return "";
	}

	cv::Mat getImg()
	{
		AutoLock lock(&CApplication::GetInstance()->m_ControlLock);

		if (!pDetectData->m_pThreadManageMentData->frame.empty())
		{
			return pDetectData->m_pThreadManageMentData->frame;
		}
		return cv::Mat();
	}

	std::string getExpression()
	{
		return pDetectData->m_pThreadManageMentData->res_exp;
	}

	void clean()
	{
		if (pDetectData != NULL)
		{
			pDetectData->m_pThreadManageMentData->bStopThread = true;
			Sleep(1000);
			pDetectData->m_pThreadManageMentData->res_json.clear();
			//pDetectData->m_pThreadManageMentData->vcap.release();

			for (int i = 0; i < THREAD_NUM; i++)
			{
				if (pDetectData->m_pThreadManageMentData->m_pThreadManageMentDataKernel[i].hDetectThread!=nullptr)
					CloseHandle(pDetectData->m_pThreadManageMentData->m_pThreadManageMentDataKernel[i].hDetectThread);
				
			}
			delete[] pDetectData->m_pThreadManageMentData->m_pThreadManageMentDataKernel;
			delete pDetectData;
		}
	}
private:

};