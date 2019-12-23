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
    ����ʶ��:
    �ӿ�������Ҫ����:
    ������������еĴʻ�����ת��Ϊ������ɶ������룬���簴���������Ʊ�������ַ�����

    ע������:
    ��Ҫ�������ýӿڡ�
    PC����ʶ��SDKĿǰ֧���ֶ��Ͼ估�����Զ��Ͼ䡣 Ŀǰϵͳ֧�ֵ�����ʱ������Ϊ60s���벻Ҫ����������ȣ�����᷵�ش���
    */

    /**
    * ʵʱ�����ص�
    * @param int ���ص�����
    * @return
    */
	typedef void(NDHYAI_CALL *onVolumeCallback)(int);

	//Vad����ʶ��ͣ�ٵǼ�
	enum VAD_PAUSE_LEVEL
	{
		VAD_PAUSE_LEVEL_NORMAL,		//1.5��
		VAD_PAUSE_LEVEL_LOW,		//1��
		VAD_PAUSE_LEVEL_HIGHT,		//2��
	};

	/**
	* ����ʶ���ܳ�ʼ���°�(�ɰ����)
	* @param CbResult        ����ص�
	* @param CbVolume        ʵʱ�����ص�
	* @param param           ��ʼ��������UTF8�����ʽ
	"{
		"baidu" : {
		"InlineBtAcount": 1,				//�Ƿ����Ĭ�ϰٶ��˺ţ�Ϊtrue��ɲ�������ٶ��˺���Ϣ���������
		"app_id" : "",						//�ٶ�appid
		"api_key" : "",						//�ٶ�apikey
		"secret_key" : "",					//�ٶ�secretkey
		"dev_pid" : 111						//����,����(����) 1(Ӣ��)
		}
	}"
	* @return                ��������ʶ��ʵ��
	*/
	NDAI_SPEECH_EXTERN void* NDHYAI_CALL Ex_SpeechRecoInitEx(
		onResultCallback CbResult,
		onVolumeCallback CbVolume,
		const char*param
		);

    /**
    * ����ʶ���ܳ�ʼ��(ʹ��ǰ����ִ��)
    * @param bInlineBtAcount �Ƿ����Ĭ�ϰٶ��˺ţ�Ϊtrue��ɲ�������ٶ��˺���Ϣ���������
    * @param app_id          �ٶ�appid
    * @param api_key         �ٶ�apikey
    * @param secret_key      �ٶ�secretkey
    * @param dev_pid         ����,����(����) en(Ӣ��)
    * @param CbResult        ����ص�
    * @param CbVolume        ʵʱ�����ص�
    * @return                ��������ʶ��ʵ��
    */

	//ʶ����������
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
    * ���٣��˳�ִ�У�
	* @param pEngine         ��Ex_SpeechRecoInit������ʵ��
    * @return
    */
	NDAI_SPEECH_EXTERN void NDHYAI_CALL Ex_SpeechRecoUnInit(void* pEngine);

    /**
    * ��ʼ¼��
	* @param pEngine         ��Ex_SpeechRecoInit������ʵ��
    * @return                true:�ɹ�  false:δ��ʼ��������������������Գ�ʼ��ʱ����Ļص����أ�
    */

	NDAI_SPEECH_EXTERN bool NDHYAI_CALL Ex_SpeechRecoStart(void* pEngine);

    /**
    * ����¼��
	* @param pEngine         ��Ex_SpeechRecoInit������ʵ��
    * @return                true:�ɹ�  false:δ��ʼ��������������������Գ�ʼ��ʱ����Ļص����أ�
    */

	NDAI_SPEECH_EXTERN bool NDHYAI_CALL Ex_SpeechRecoStop(void* pEngine);

    /**
    * ��ʼ¼����vad��
	* @param pEngine         ��Ex_SpeechRecoInit������ʵ��
    * @return                true:�ɹ�  false:δ��ʼ��������������������Գ�ʼ��ʱ����Ļص����أ�
    */

	NDAI_SPEECH_EXTERN bool NDHYAI_CALL Ex_SpeechRecoStartVad(void* pEngine);

    /**
    * ����¼����vad��
	* @param pEngine         ��Ex_SpeechRecoInit������ʵ��
    * @return                true:�ɹ�  false:δ��ʼ��������������������Գ�ʼ��ʱ����Ļص����أ�
    */
	NDAI_SPEECH_EXTERN bool NDHYAI_CALL Ex_SpeechRecoStopVad(void* pEngine);

    /**
    * ʶ�������ļ��ӿڣ�ͬ����
    * @return                true:�ɹ�  false:δ��ʼ��������������������Գ�ʼ��ʱ����Ļص����أ�
	* @param pEngine         ��Ex_SpeechRecoInit������ʵ��
    * @param sAduioPath      ��Ƶ�ļ�����·����Ŀǰ֧��WAV,PCM,AMR��MP3��ʽ��Ƶ�ļ�,��ͨ����16λ��;AMR,PCM��ʽ�ļ���֧��16000�����ʣ�������ʽ֧��16000��8000�����ʣ�UTF8�����ʽ
	* @param bffmpeg         �Ƿ�����ffmego��ת��
    */
	NDAI_SPEECH_EXTERN int NDHYAI_CALL Ex_SpeechRecoByFileSyn(void* pEngine, const char *sAduioPath, bool bffmpeg, onResultCallback_1 CbResult, const void *userdata);

	/**
	* ���û��ѹ��ܣ�ֻ��vadʶ����Ч,֧�ֻ��Ѵ��Զ��壨awakenwords�ļ����ã���
	1���ĵ�ֻ��ʹ��uft-8�����ʽ��ʹ��Ĭ�ϵ��ĵ��������������޸ģ�
	2���ؼ����������ʮ��
	3��ÿ���ؼ������ò�������100����Ŀǰʹ�����Ի��ѹ����н��鲻Ҫ����20��������ѻ��ѣ�
	4���ؼ��ʱ�����ֻ�����������ģ��������֡�ƴ����Ӣ�ġ������ַ�������������ڣ�
	5������֮������һֱ������������֮ǰδʹ�û��ѹ����¼��60S�����뻽�ѹ���֮���¼��55�룬��˽������������ܹ�һ�仰��ɣ�
	6������֮��ֻ��������һ��������֮����Ҫ���»��Ѳſ���

	* @param pEngine         ��Ex_SpeechRecoInit������ʵ��
	* @param bAwaken         �������ѱ�־��true ����  false �ر�
	* @return                true:�ɹ�  false:δ��ʼ��������������������Գ�ʼ��ʱ����Ļص����أ�
	*/

	NDAI_SPEECH_EXTERN bool NDHYAI_CALL Ex_SpeechAwaken(void* pEngine, bool bAwaken);

#ifdef __cplusplus
}
#endif