#include "stdafx.h"
#include "qFaceExpression.h"

size_t write_data(void *ptr, size_t size, size_t nmemb, void *stream)
{
	string data((const char*)ptr, (size_t)size * nmemb);

	*((stringstream*)stream) << data << endl;

	return size * nmemb;
}

std::string img2base64(cv::Mat &img)
{
	if (img.empty())
		return "";
	vector<uchar> buf;
	cv::imencode(".jpg", img, buf);
	uchar *enc_msg = new uchar[buf.size()];
	for (int i = 0; i < int(buf.size()); i++)
	{
		enc_msg[i] = buf[i];
	}
	std::string str_img = base64_encode(enc_msg, buf.size());
	delete[] enc_msg;
	return str_img;
}

unsigned int __stdcall faceDetect(PVOID pM)
{
	int ret_code = 0;
	ThreadManageMentData * pThreadData = (ThreadManageMentData*)pM;
	//std::string postUrlStr = "http://192.168.46.111:3600/face/detection";
	std::string postUrlStr = "https://eamwa5m3i8.execute-api.cn-north-1.amazonaws.com.cn/face-recognition-v1-release/face/detection";
	CURL *curl = NULL;
	struct curl_slist *headers = NULL;
	CURLcode result_code;
	// int is_success;
	curl = curl_easy_init();
	if (curl)
	{
		std::stringstream str_post;
		cv::Mat frame = pThreadData->image;
		string str_data = img2base64(frame(pThreadData->box));
		Json::Value value;
		value["upl_img"] = str_data;
		Json::StreamWriterBuilder swb;
		std::string postParams = Json::writeString(swb, value);
		curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "POST");
		curl_easy_setopt(curl, CURLOPT_URL, postUrlStr.c_str());
		headers = curl_slist_append(headers, "cache-control: no-cache");
		headers = curl_slist_append(headers, "Content-Type:application/json;charset=UTF-8");
		headers = curl_slist_append(headers, "x-api-key: cdER11GGkn9Mpea4QXx7k9L1brAL53PE3LnCPuZS");
		headers = curl_slist_append(headers, "api_key: 9C0F8B3C76B411E7B0E7DC4A3E7461E5");
		headers = curl_slist_append(headers, "api_secret: 9C0F8B9D76B411E7B0E7DC4A3E7461E5");
		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
		//curl_easy_setopt(curl, CURLOPT_POSTFIELDS, "{\"img_url\": \"http://betacs.101.com/v0.1/static/qa_content_invigilate/skin/null/1576656111049/txq_123456.png\"}");
		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postParams.c_str());
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &str_post);
		result_code = curl_easy_perform(curl);
		//std::cout << result_code << std::endl;
		if (result_code != CURLE_OK)
		{
			cerr << "curl_easy_perform() failed: " + string(curl_easy_strerror(result_code)) << endl;
			ret_code = 0;
		}
		else
		{
			ret_code = 1;
			pThreadData->res_json.clear();
			//Json::Value value_json;
			Json::CharReaderBuilder b;
			Json::CharReader* reader_json(b.newCharReader());
			string str_json = str_post.str();
			JSONCPP_STRING errs;
			bool ok = reader_json->parse(str_json.data(), str_json.data() + std::strlen(str_json.data()), &(pThreadData->res_json), &errs);
			//std::cout << str_json << std::endl;
		}
		value.clear();
		if (!str_post.str().empty())
		{
			str_post.clear();
			str_post.str("");
		}
	}
	curl_slist_free_all(headers);
	curl_easy_cleanup(curl);
	pThreadData->bStart = true;

	curl_global_cleanup();
	//curl_free();
	_endthreadex(0);
	return ret_code;
}


unsigned int __stdcall kinect_image(PVOID pM)
{
	//https://elody-07.github.io/kinect
	IKinectSensor   * mySensor = nullptr;
	GetDefaultKinectSensor(&mySensor);
	mySensor->Open();

	IColorFrameSource   * myColorSource = nullptr;
	mySensor->get_ColorFrameSource(&myColorSource);
	IColorFrameReader   * myColorReader = nullptr;
	myColorSource->OpenReader(&myColorReader);

	int colorHeight = 0, colorWidth = 0;
	IFrameDescription   * myDescription = nullptr;
	myColorSource->get_FrameDescription(&myDescription);
	myDescription->get_Height(&colorHeight);
	myDescription->get_Width(&colorWidth);
	int im_H = colorHeight / 2;
	int im_W = colorWidth / 2;
	IColorFrame * myColorFrame = nullptr;
	cv::Mat original(colorHeight, colorWidth, CV_8UC4);
	DetectData *pDetectData = (DetectData*)pM;
	ThreadManageMentData * pThreadData = (ThreadManageMentData*)pDetectData->m_pThreadManageMentData;
	pThreadData->box = cv::Rect(int(im_W / 4), int(im_H / 6), int(im_W / 2), int(im_H * 2 / 3));

	//**********************以上为ColorFrame的读取前准备**************************
	cv::Rect box = pThreadData->box;
	cv::Mat frame;
	bool state_code = false;
	int c = 1, timeF = 15;
	bool is_ok = false;
	//clock_t start = clock();
	while (1)
	{
		if (pThreadData->bStopThread)
			break;
		while (myColorReader->AcquireLatestFrame(&myColorFrame) != S_OK);
		myColorFrame->CopyConvertedFrameDataToArray(colorHeight * colorWidth * 4, original.data, ColorImageFormat_Bgra);
		cv::resize(original, frame, cv::Size(im_W, im_H));
		cv::cvtColor(frame, frame, cv::COLOR_RGBA2RGB);
		myColorFrame->Release();
		std::string exp_str = pThreadData->ref_exp;
		/*if (frame.empty())
		{
		cout << "data is empty." << endl;
		break;
		}*/
		// face detection --> api finish
		if (c % int(timeF / 5) == 0)
		{
			AutoLock lock(&CApplication::GetInstance()->m_ControlLock);

			cv::rectangle(frame, box, cv::Scalar(0, 0, 255));
			pThreadData->frame = frame.clone();
		}

		if (c % timeF == 0 && !state_code)
		{
			state_code = true;
			pThreadData->image = frame.clone();
			pThreadData->nThreadIndex = 1;
			pThreadData->m_pThreadManageMentDataKernel[1].hStartEvent = CreateEvent(NULL, false, false, NULL);
			pThreadData->m_pThreadManageMentDataKernel[1].hEndEvent = CreateEvent(NULL, false, false, NULL);
			pThreadData->m_pThreadManageMentDataKernel[1].hDetectThread = (HANDLE)_beginthreadex(NULL, 0, faceDetect, (void*)pThreadData, 0, NULL);
		}
		if (state_code)
		{
			if (pThreadData->bStart)
			{
				pThreadData->bStart = false;
				state_code = false;
				if (!pThreadData->bStopThread)
					CloseHandle(pThreadData->m_pThreadManageMentDataKernel[1].hDetectThread);
				int face_num = pThreadData->res_json["faceNum"].asInt();
				if (face_num > 0)
				{
					string expr = pThreadData->res_json["faces_info"][0]["expression"].asString();
					pThreadData->res_exp = expr;
				}
				else
				{
					pThreadData->res_exp = "";
				}

				if (exp_str.compare(pThreadData->res_exp) == 0)
				{
					/*std::cout << "ok" << std::endl;
					is_ok = true;
					break;*/
				}
			}
			/*else
			{
			pThreadData->res_exp = "";
			}*/
		}
		c++;
		if (c == 1024)
			c = 0;

	}
	WaitForSingleObject(pThreadData->m_pThreadManageMentDataKernel[1].hEndEvent, INFINITE);
	myDescription->Release();
	myColorReader->Release();
	myColorSource->Release();
	mySensor->Close();
	mySensor->Release();
	_endthreadex(0);
	return  0;
}


unsigned int __stdcall cameraData(PVOID pM)
{
	DetectData *pDetectData = (DetectData*)pM;
	ThreadManageMentData * pThreadData = (ThreadManageMentData*)pDetectData->m_pThreadManageMentData;
	cv::VideoCapture capture = pThreadData->vcap;
	cv::Rect box = pThreadData->box;
	cv::Mat frame;
	bool state_code = false;
	int c = 1, timeF = 25;
	bool is_ok = false;
	while (1)
	{
		std::string exp_str = pThreadData->ref_exp;
		capture >> frame;
		if (frame.empty())
		{
			cout << "data is empty." << endl;
			break;
		}
		// face detection --> api finish
		if (c % timeF == 0 && !state_code)
		{
			//faceDetect(frame(box));
			state_code = true;
			cv::rectangle(frame, box, cv::Scalar(0, 0, 255));
			pThreadData->image = frame.clone();
			pThreadData->nThreadIndex = 1;
			pThreadData->m_pThreadManageMentDataKernel[1].hStartEvent = CreateEvent(NULL, false, false, NULL);
			pThreadData->m_pThreadManageMentDataKernel[1].hEndEvent = CreateEvent(NULL, false, false, NULL);
			pThreadData->m_pThreadManageMentDataKernel[1].hDetectThread = (HANDLE)_beginthreadex(NULL, 0, faceDetect, (void*)pThreadData, 0, NULL);
		}
		if (state_code)
		{
			if (pThreadData->bStart)
			{
				pThreadData->bStart = false;
				state_code = false;
				CloseHandle(pThreadData->m_pThreadManageMentDataKernel[1].hDetectThread);
				int face_num = pThreadData->res_json["faceNum"].asInt();
				if (face_num > 0)
				{
					string expr = pThreadData->res_json["faces_info"][0]["expression"].asString();
					pThreadData->res_exp = expr;
				}
				else
				{
					pThreadData->res_exp = "";
				}

				if (exp_str.compare(pThreadData->res_exp) == 0)
				{
					/*std::cout << "ok" << std::endl;
					is_ok = true;
					break;*/
				}
			}
		}
		c++;
		if (c == 1024)
			c = 0;

	}
	//}
	WaitForSingleObject(pDetectData->m_pThreadManageMentData->m_pThreadManageMentDataKernel[1].hEndEvent, INFINITE);
	capture.release();

	return 1;
}

FaceExpression::FaceExpression()
{

}

FaceExpression::~FaceExpression()
{

}

int FaceExpression::init(std::string str_exp)
{
	exp_str = str_exp;
	curl_global_init(CURL_GLOBAL_ALL);
	pDetectData = new DetectData;
	//多线程程序
	pDetectData->m_pThreadManageMentData = new ThreadManageMentData;
	memset(pDetectData->m_pThreadManageMentData, 0x00, sizeof(ThreadManageMentData));

	pDetectData->m_pThreadManageMentData->m_nKernelThreadCount = THREAD_NUM;
	pDetectData->m_pThreadManageMentData->bStopThread = false;
	pDetectData->m_pThreadManageMentData->m_pThreadManageMentDataKernel = new ThreadManageMentDataKernel[THREAD_NUM];
	memset(pDetectData->m_pThreadManageMentData->m_pThreadManageMentDataKernel, 0x00, sizeof(ThreadManageMentDataKernel)*THREAD_NUM);

	//open default camera
	//cv::VideoCapture capture(0);
	//double fps = capture.get(cv::CAP_PROP_FPS);   //获取视频或者摄像头读取的视频流的帧率
	//capture.set(cv::CAP_PROP_POS_FRAMES, 50);
	//double c_w = capture.get(CV_CAP_PROP_FRAME_WIDTH);
	//double c_h = capture.get(CV_CAP_PROP_FRAME_HEIGHT);
	//// check whether open the camera
	//if (!capture.isOpened())
	//{
	//	cout << "open camera is failed." << endl;
	//	return -1;
	//}
	//pDetectData->m_pThreadManageMentData->box = cv::Rect(int(c_w / 4), int(c_h / 6), int(c_w / 2), int(c_h * 2 / 3));
	pDetectData->m_pThreadManageMentData->nThreadIndex = 0;
	//pDetectData->m_pThreadManageMentData->vcap = capture;
	pDetectData->m_pThreadManageMentData->m_pThreadManageMentDataKernel[0].hStartEvent = CreateEvent(NULL, false, false, NULL);
	pDetectData->m_pThreadManageMentData->m_pThreadManageMentDataKernel[0].hEndEvent = CreateEvent(NULL, false, false, NULL);
	pDetectData->m_pThreadManageMentData->m_pThreadManageMentDataKernel[0].hDetectThread = (HANDLE)_beginthreadex(NULL, 0, kinect_image, (void*)pDetectData, 0, NULL);
	return 1;
}