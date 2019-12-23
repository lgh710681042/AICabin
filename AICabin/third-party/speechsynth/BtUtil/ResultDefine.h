#pragma once

/**
* 识别返回码
* @return
*/

#if defined(WIN32) || defined(_WIN32) || defined(_WIN64) || defined(__SYMBIAN32__)
#  define NDHYAI_CALL __stdcall
#else
#  define NDHYAI_CALL
#endif

extern "C"{
enum  struct RESULT_TYPE
{
    SUCESS,                                      //成功
	INTENT_PROCESS,                              //返回各识别环节信息
	RECO_VAD_START,								 //语音VAD开始标志
	RECO_VAD_END,								 //语音VAD结束标志
	VOICE_CORE_SERVICE_MSG,                      //驰声评测返回信息
	RECO_WAKEN_BEGIN,							 //语音唤醒开始
	RECO_WAKEN_END,								 //语音唤醒结束
    //人脸识别失败
    FACE_ERROR = 1000,
	FACE_NOT_INIT,                                //未初始化
	FACE_PARAM_ERROR,
    ERROR_TASK_TOO_MUCH,                         //待处理任务过多  
    FACE_QUALITY_ATYPIA,                         //图片质量不符合要求
    FACE_ERROR_SOUR_FILE_OPEN,                   //原图片文件打开失败    
    FACE_ERROR_REQUEST,                          //请求失败， 具体失败类型存放msg 
    FACE_ERROR_REQUEST_EX,                       //请求失败（curl错误）， 具体失败类型存放msg
    FACE_ERROR_NO_COLLECT_CAMERA,                //未连接摄像头  
    FACE_ERROR_COLLECT_LOAD_CONFIG,              //采集opencv配置文件加载失败（文件不存在或中文路径）  
    //语音识别
    RECO_ERROR = 2000,
	RECO_ERROR_CREATE_PATH,
    RECO_ERROR_NO_DEVICE,                        //未检测到录音设备
	RECO_ERROR_HAS_OPENED,                       //已处于录音状态无法再次开启
    RECO_ERROR_INIT,
    RECO_ERROR_REQUEST,
    RECO_ERROR_REQUEST_EX,
    RECO_ERROR_PARAM,                            //参数错误
	RECO_XF,									 //XF语音识别失败
	RECO_ERROR_LOAD_FFMPEG,                       //ffmpeg库加载异常
	RECO_ERROR_LOAD_XF,                           //XF库加载异常
	RECO_ERROR_LOAD_PINYIN,                       //pinyin库加载异常
	RECO_ERROR_AWAKEN,							  //唤醒加载异常（需pinyin.dll和awakenwords）
    //OCR
    OCR_ERROR = 3000,
	OCR_NOT_INIT,
	OCR_PARAM_ERROR,
    OCR_ERROR_TASK_TOO_MUCH,                    //待处理任务过多  
    OCR_ERROR_SOUR_FILE_OPEN,                   //原图片文件打开失败    
    OCR_ERROR_REQUEST,                          //请求失败， 具体失败类型存放msg
    OCR_ERROR_REQUEST_EX,                       //请求失败（curl错误）， 具体失败类型存放msg
    //手写
    OCR_EEROR_SINGLE_PARAM_STR_POINTS,          //点参数错误
    OCR_EEROR_SINGLE_MODELS_FILE_INIT,          //初始化识别        
    //语音合成
    SYNTH_ERROR = 4000,
    SYNTH_ERROR_TASK_TOO_MUCH,                //待处理任务过多  
    SYNTH_ERROR_DEST_FILE_OPEN,               //目标音频文件打开失败    
    SYNTH_ERROR_REQUEST,                      //合成失败  
    SYNTH_ERROR_REQUEST_EX,                   //合成失败 
    //意图解析
    INTENT_ERROR = 5000,
	INTENT_NOT_INIT,						 //实例未初始化
	INTENT_PARAM_ERROR,						 //传入参数错误
    INTENT_PPTAI_PERSONL_ERROR,              //编辑器语料接口返回错误
    INTENT_PPTAI_NO_MATCH,                   //编辑器返回数据未匹配到自定义语料
    INTENT_SERVICE_ERROR,                    //访问服务器接口返回错误
    INTENT_SERVICE_NO_MATCH,                 //编辑器未匹配到语料
    INTENT_SERVICE_TOKEN,                    //无效token
    INTENT_IFLYTEK_ERRPR,                    //讯飞接口未匹配到语料或返回其他错误
    INTENT_PPTAI_CHAT_ERROR,                 //天晴猪聊天接口错误

    //翻译
    TRANSLATOR_ERROR = 6000,                 //翻译错误
	TRANSLATOR_NOT_INIT,                 
	TRANSLATOR_PARAM_ERROR,

    //驰声语音评测
	VOICE_EVALUAT_ERROR = 7000,              //语音评测
	VOICE_EVALUAT_ERROR_PARAM = 7001,        //参数传入错误
	VOICE_EVALUAT_ERROR_NO_INIT = 7002,       //SDK未初始化
	VOICE_EVALUAT_ERROR_CORE_SERVICE_NEW = 7003,       //CORESERVICE创建失败
	VOICE_EVALUAT_ERROR_AUDIO_FILE = 7004,       //传入的音频文件打开失败/格式有误

	//图片检索
	IMAGE_SEARCH_ERROR = 8000,
	IMAGE_SEARCH_NOT_INIT,
	IMAGE_SEARCH_PARAM_ERROR,
	IMAGE_SEARCH_ERROR_TASK_TOO_MUCH,                    //待处理任务过多  
	IMAGE_SEARCH_ERROR_SOUR_FILE_OPEN,                   //原图片文件打开失败    
	IMAGE_SEARCH_ERROR_REQUEST,                          //请求失败， 具体失败类型存放msg
	IMAGE_SEARCH_ERROR_REQUEST_EX,                       //请求失败（curl错误）， 具体失败类型存放msg

	//ffmpeg转换
	FFMPEG_ERROR = 9000,
};

/**
* 识别返回结果数据
* @return
*/
struct ResultData
{
    RESULT_TYPE dwCode = RESULT_TYPE::SUCESS;         //返回接口, =0:识别正确，>=1000,错误
    std::string strMsg;                               //返回具体信息,dwCode=0:识别结果  dwCode>=1000:错误信息
    double dwTimep = 0;                               //识别接口所用的时间
};

#define MAX_MSG_LEN 51200

struct ResultDataEx
{
    RESULT_TYPE dwCode;               //返回接口, =0:识别正确，>=1000,错误
    char strMsg[MAX_MSG_LEN];         //返回具体信息,dwCode=0:识别结果  dwCode>=1000:错误信息
};

/**
* 结果回调
* @param int 错误码  0为正确，其他为错误
* @param char*  第一个参数为0时为返回的数据，>1000为错误信息
* @return
*/
typedef void(NDHYAI_CALL *onResultCallback)(int, char *);
typedef int (NDHYAI_CALL *onResultCallback_1)(const void *usrdata, int result, const void *data, int size);
}