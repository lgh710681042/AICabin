#pragma once

#ifdef NDHYAI_STATICLIB
#  define NDAI_SPEECH_SYNTH_EXTERN
#elif defined(WIN32) || defined(_WIN32) || defined(__SYMBIAN32__)
#  if defined(NDAI_SPEECH_SYNTH_EXPORT)
#    define NDAI_SPEECH_SYNTH_EXTERN  __declspec(dllexport)
#  else
#    define NDAI_SPEECH_SYNTH_EXTERN  __declspec(dllimport)
#  endif
#else
#  define NDAI_SPEECH_SYNTH_EXTERN
#endif

extern "C"{
    /*
    语音合成:
    接口能力简要描述:
    将计算机自己产生的、或外部输入的文字信息转变为可以听得懂的、流利的口语输出的技术

    注意事项:
    需要联网调用接口。
    支持最多512字（1024 字节)的音频合成，合成的文件格式为mp3。
    */

    /**
    * 语音合成功能初始化(使用前必须执行)
    * @param bInlineBtAcount 是否采用默认百度账号，为true则可不填后续百度账号信息，否则必填
    * @param app_id          百度appid
    * @param api_key         百度apikey
    * @param secret_key      百度secretkey
    * @return                true:成功   false:初始化失败或参数错误
    */

	NDAI_SPEECH_SYNTH_EXTERN bool NDHYAI_CALL Ex_SpeechSynthInit(
        bool        bInlineBtAcount = true,
        const char app_id[] = "",
		const char api_key[] = "",
		const char secret_key[] = ""
        );

    /**
    * 销毁
    * @return
    */
	NDAI_SPEECH_SYNTH_EXTERN void NDHYAI_CALL Ex_SpeechSynthUnInit();

    /**
    * 开始合成
    * @param strText          待合成的文本
    * @param strFilePath      合成的目标音频文件路径,合成的文件格式为mp3
	* @param strper           发音人选择, 0为女声，1为男声，3为情感合成-度逍遥，4为情感合成-度丫丫，默认为普通女
    * @param CbResult         结果回调
    * @return                 true:成功  false:未初始化未初始化或参数错误（其他错误以回调返回）
    */

	NDAI_SPEECH_SYNTH_EXTERN bool NDHYAI_CALL Ex_SpeechSynthStartAsyn(const char *strText, const char *strFilePath, const char *strper = "0", onResultCallback CbResult = NULL);

    /**
    * 结束合成（未实现）
    * @return                 true:成功  false:未初始化
    */
	NDAI_SPEECH_SYNTH_EXTERN bool NDHYAI_CALL Ex_SpeechSynthStop();
}

