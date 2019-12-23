#pragma once

#ifdef NDHYAI_STATICLIB
#  define NDAI_SPEECH_EXTERN
#elif defined(_WIN32) || defined(WIN32)
#  if defined(NDAI_SPEECH_EXPORT)
#    define NDAI_SPEECH_EXTERN  __declspec(dllexport)
#  else
#    define NDAI_SPEECH_EXTERN  __declspec(dllimport)
#  endif
#else
#  define NDAI_SPEECH_EXTERN
#endif

#ifdef __cplusplus
extern "C"{
#endif
    /*
    语音识别:
    接口能力简要描述:
    将人类的语音中的词汇内容转换为计算机可读的输入，例如按键、二进制编码或者字符序列

    注意事项:
    需要联网调用接口。
    PC语音识别SDK目前支持手动断句及静音自动断句。 目前系统支持的语音时长上限为60s，请不要超过这个长度，否则会返回错误。
    */

    /**
    * 实时音量回调
    * @param int 返回的音量
    * @return
    */
	typedef void(NDHYAI_CALL *onVolumeCallback)(int);

	//Vad语音识别停顿登记
	enum VAD_PAUSE_LEVEL
	{
		VAD_PAUSE_LEVEL_NORMAL,		//1.5秒
		VAD_PAUSE_LEVEL_LOW,		//1秒
		VAD_PAUSE_LEVEL_HIGHT,		//2秒
	};

	/**
	* 语音识别功能初始化新版(旧版兼容)
	* @param CbResult        结果回调
	* @param CbVolume        实时音量回调
	* @param param           初始化参数，UTF8编码格式
	"{
		"baidu" : {
		"InlineBtAcount": 1,				//是否采用默认百度账号，为true则可不填后续百度账号信息，否则必填
		"app_id" : "",						//百度appid
		"api_key" : "",						//百度apikey
		"secret_key" : "",					//百度secretkey
		"dev_pid" : 111						//语言,其他(中文) 1(英语)
		}
	}"
	* @return                返回语音识别实例
	*/
	NDAI_SPEECH_EXTERN void* NDHYAI_CALL Ex_SpeechRecoInitEx(
		onResultCallback CbResult,
		onVolumeCallback CbVolume,
		const char*param
		);

    /**
    * 语音识别功能初始化(使用前必须执行)
    * @param bInlineBtAcount 是否采用默认百度账号，为true则可不填后续百度账号信息，否则必填
    * @param app_id          百度appid
    * @param api_key         百度apikey
    * @param secret_key      百度secretkey
    * @param dev_pid         语言,其他(中文) en(英语)
    * @param CbResult        结果回调
    * @param CbVolume        实时音量回调
    * @return                返回语音识别实例
    */

	//识别语音类型
	enum DEV_PID
	{
		CN,
		EN
	};

	NDAI_SPEECH_EXTERN void* NDHYAI_CALL Ex_SpeechRecoInit(
        onResultCallback CbResult,
        onVolumeCallback CbVolume,
        bool        bInlineBtAcount = true,
        const char *app_id = "",
		const char *api_key = "",
		const char *secret_key = "",
		DEV_PID dev_pid = CN
        );

    /**
    * 销毁（退出执行）
	* @param pEngine         由Ex_SpeechRecoInit创建的实例
    * @return
    */
	NDAI_SPEECH_EXTERN void NDHYAI_CALL Ex_SpeechRecoUnInit(void* pEngine);

    /**
    * 开始录音
	* @param pEngine         由Ex_SpeechRecoInit创建的实例
    * @return                true:成功  false:未初始化或参数错误（其他错误以初始化时传入的回调返回）
    */

	NDAI_SPEECH_EXTERN bool NDHYAI_CALL Ex_SpeechRecoStart(void* pEngine);

    /**
    * 结束录音
	* @param pEngine         由Ex_SpeechRecoInit创建的实例
    * @return                true:成功  false:未初始化或参数错误（其他错误以初始化时传入的回调返回）
    */

	NDAI_SPEECH_EXTERN bool NDHYAI_CALL Ex_SpeechRecoStop(void* pEngine);

    /**
    * 开始录音（vad）
	* @param pEngine         由Ex_SpeechRecoInit创建的实例
    * @return                true:成功  false:未初始化或参数错误（其他错误以初始化时传入的回调返回）
    */

	NDAI_SPEECH_EXTERN bool NDHYAI_CALL Ex_SpeechRecoStartVad(void* pEngine);

    /**
    * 结束录音（vad）
	* @param pEngine         由Ex_SpeechRecoInit创建的实例
    * @return                true:成功  false:未初始化或参数错误（其他错误以初始化时传入的回调返回）
    */
	NDAI_SPEECH_EXTERN bool NDHYAI_CALL Ex_SpeechRecoStopVad(void* pEngine);

    /**
    * 识别语音文件接口（同步）
    * @return                true:成功  false:未初始化或参数错误（其他错误以初始化时传入的回调返回）
	* @param pEngine         由Ex_SpeechRecoInit创建的实例
    * @param sAduioPath      音频文件完整路径，目前支持WAV,PCM,AMR，MP3格式音频文件,单通道，16位深;AMR,PCM格式文件仅支持16000采样率，其他格式支持16000和8000采样率，UTF8编码格式
	* @param bffmpeg         是否引入ffmego库转换
    */
	NDAI_SPEECH_EXTERN int NDHYAI_CALL Ex_SpeechRecoByFileSyn(void* pEngine, const char *sAduioPath, bool bffmpeg, onResultCallback_1 CbResult, const void *userdata);

	/**
	* 设置唤醒功能，只在vad识别有效,支持唤醒词自定义（awakenwords文件配置）；
	1、文档只能使用uft-8编码格式（使用默认的文档进行新增或者修改）
	2、关键词最多设置十个
	3、每个关键词设置不允许超过100个（目前使用语言唤醒过程中建议不要超过20个否则很难唤醒）
	4、关键词表里面只允许输入中文（其他数字、拼音、英文、特殊字符将不会计算在内）
	5、唤醒之后不允许一直在输入语音（之前未使用唤醒功能最长录制60S、加入唤醒功能之后最长录制55秒，因此建议语音输入能够一句话完成）
	6、唤醒之后只允许输入一次语音，之后需要重新唤醒才可以

	* @param pEngine         由Ex_SpeechRecoInit创建的实例
	* @param bAwaken         开启唤醒标志：true 开启  false 关闭
	* @return                true:成功  false:未初始化或参数错误（其他错误以初始化时传入的回调返回）
	*/

	NDAI_SPEECH_EXTERN bool NDHYAI_CALL Ex_SpeechAwaken(void* pEngine, bool bAwaken);

#ifdef __cplusplus
}
#endif