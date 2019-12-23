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
    �����ϳ�:
    �ӿ�������Ҫ����:
    ��������Լ������ġ����ⲿ�����������Ϣת��Ϊ�������ö��ġ������Ŀ�������ļ���

    ע������:
    ��Ҫ�������ýӿڡ�
    ֧�����512�֣�1024 �ֽ�)����Ƶ�ϳɣ��ϳɵ��ļ���ʽΪmp3��
    */

    /**
    * �����ϳɹ��ܳ�ʼ��(ʹ��ǰ����ִ��)
    * @param bInlineBtAcount �Ƿ����Ĭ�ϰٶ��˺ţ�Ϊtrue��ɲ�������ٶ��˺���Ϣ���������
    * @param app_id          �ٶ�appid
    * @param api_key         �ٶ�apikey
    * @param secret_key      �ٶ�secretkey
    * @return                true:�ɹ�   false:��ʼ��ʧ�ܻ��������
    */

	NDAI_SPEECH_SYNTH_EXTERN bool NDHYAI_CALL Ex_SpeechSynthInit(
        bool        bInlineBtAcount = true,
        const char app_id[] = "",
		const char api_key[] = "",
		const char secret_key[] = ""
        );

    /**
    * ����
    * @return
    */
	NDAI_SPEECH_SYNTH_EXTERN void NDHYAI_CALL Ex_SpeechSynthUnInit();

    /**
    * ��ʼ�ϳ�
    * @param strText          ���ϳɵ��ı�
    * @param strFilePath      �ϳɵ�Ŀ����Ƶ�ļ�·��,�ϳɵ��ļ���ʽΪmp3
	* @param strper           ������ѡ��, 0ΪŮ����1Ϊ������3Ϊ��кϳ�-����ң��4Ϊ��кϳ�-��ѾѾ��Ĭ��Ϊ��ͨŮ
    * @param CbResult         ����ص�
    * @return                 true:�ɹ�  false:δ��ʼ��δ��ʼ��������������������Իص����أ�
    */

	NDAI_SPEECH_SYNTH_EXTERN bool NDHYAI_CALL Ex_SpeechSynthStartAsyn(const char *strText, const char *strFilePath, const char *strper = "0", onResultCallback CbResult = NULL);

    /**
    * �����ϳɣ�δʵ�֣�
    * @return                 true:�ɹ�  false:δ��ʼ��
    */
	NDAI_SPEECH_SYNTH_EXTERN bool NDHYAI_CALL Ex_SpeechSynthStop();
}

