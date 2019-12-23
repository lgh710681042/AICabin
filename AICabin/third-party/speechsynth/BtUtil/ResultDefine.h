#pragma once

/**
* ʶ�𷵻���
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
    SUCESS,                                      //�ɹ�
	INTENT_PROCESS,                              //���ظ�ʶ�𻷽���Ϣ
	RECO_VAD_START,								 //����VAD��ʼ��־
	RECO_VAD_END,								 //����VAD������־
	VOICE_CORE_SERVICE_MSG,                      //�������ⷵ����Ϣ
	RECO_WAKEN_BEGIN,							 //�������ѿ�ʼ
	RECO_WAKEN_END,								 //�������ѽ���
    //����ʶ��ʧ��
    FACE_ERROR = 1000,
	FACE_NOT_INIT,                                //δ��ʼ��
	FACE_PARAM_ERROR,
    ERROR_TASK_TOO_MUCH,                         //�������������  
    FACE_QUALITY_ATYPIA,                         //ͼƬ����������Ҫ��
    FACE_ERROR_SOUR_FILE_OPEN,                   //ԭͼƬ�ļ���ʧ��    
    FACE_ERROR_REQUEST,                          //����ʧ�ܣ� ����ʧ�����ʹ��msg 
    FACE_ERROR_REQUEST_EX,                       //����ʧ�ܣ�curl���󣩣� ����ʧ�����ʹ��msg
    FACE_ERROR_NO_COLLECT_CAMERA,                //δ��������ͷ  
    FACE_ERROR_COLLECT_LOAD_CONFIG,              //�ɼ�opencv�����ļ�����ʧ�ܣ��ļ������ڻ�����·����  
    //����ʶ��
    RECO_ERROR = 2000,
	RECO_ERROR_CREATE_PATH,
    RECO_ERROR_NO_DEVICE,                        //δ��⵽¼���豸
	RECO_ERROR_HAS_OPENED,                       //�Ѵ���¼��״̬�޷��ٴο���
    RECO_ERROR_INIT,
    RECO_ERROR_REQUEST,
    RECO_ERROR_REQUEST_EX,
    RECO_ERROR_PARAM,                            //��������
	RECO_XF,									 //XF����ʶ��ʧ��
	RECO_ERROR_LOAD_FFMPEG,                       //ffmpeg������쳣
	RECO_ERROR_LOAD_XF,                           //XF������쳣
	RECO_ERROR_LOAD_PINYIN,                       //pinyin������쳣
	RECO_ERROR_AWAKEN,							  //���Ѽ����쳣����pinyin.dll��awakenwords��
    //OCR
    OCR_ERROR = 3000,
	OCR_NOT_INIT,
	OCR_PARAM_ERROR,
    OCR_ERROR_TASK_TOO_MUCH,                    //�������������  
    OCR_ERROR_SOUR_FILE_OPEN,                   //ԭͼƬ�ļ���ʧ��    
    OCR_ERROR_REQUEST,                          //����ʧ�ܣ� ����ʧ�����ʹ��msg
    OCR_ERROR_REQUEST_EX,                       //����ʧ�ܣ�curl���󣩣� ����ʧ�����ʹ��msg
    //��д
    OCR_EEROR_SINGLE_PARAM_STR_POINTS,          //���������
    OCR_EEROR_SINGLE_MODELS_FILE_INIT,          //��ʼ��ʶ��        
    //�����ϳ�
    SYNTH_ERROR = 4000,
    SYNTH_ERROR_TASK_TOO_MUCH,                //�������������  
    SYNTH_ERROR_DEST_FILE_OPEN,               //Ŀ����Ƶ�ļ���ʧ��    
    SYNTH_ERROR_REQUEST,                      //�ϳ�ʧ��  
    SYNTH_ERROR_REQUEST_EX,                   //�ϳ�ʧ�� 
    //��ͼ����
    INTENT_ERROR = 5000,
	INTENT_NOT_INIT,						 //ʵ��δ��ʼ��
	INTENT_PARAM_ERROR,						 //�����������
    INTENT_PPTAI_PERSONL_ERROR,              //�༭�����Ͻӿڷ��ش���
    INTENT_PPTAI_NO_MATCH,                   //�༭����������δƥ�䵽�Զ�������
    INTENT_SERVICE_ERROR,                    //���ʷ������ӿڷ��ش���
    INTENT_SERVICE_NO_MATCH,                 //�༭��δƥ�䵽����
    INTENT_SERVICE_TOKEN,                    //��Чtoken
    INTENT_IFLYTEK_ERRPR,                    //Ѷ�ɽӿ�δƥ�䵽���ϻ򷵻���������
    INTENT_PPTAI_CHAT_ERROR,                 //����������ӿڴ���

    //����
    TRANSLATOR_ERROR = 6000,                 //�������
	TRANSLATOR_NOT_INIT,                 
	TRANSLATOR_PARAM_ERROR,

    //������������
	VOICE_EVALUAT_ERROR = 7000,              //��������
	VOICE_EVALUAT_ERROR_PARAM = 7001,        //�����������
	VOICE_EVALUAT_ERROR_NO_INIT = 7002,       //SDKδ��ʼ��
	VOICE_EVALUAT_ERROR_CORE_SERVICE_NEW = 7003,       //CORESERVICE����ʧ��
	VOICE_EVALUAT_ERROR_AUDIO_FILE = 7004,       //�������Ƶ�ļ���ʧ��/��ʽ����

	//ͼƬ����
	IMAGE_SEARCH_ERROR = 8000,
	IMAGE_SEARCH_NOT_INIT,
	IMAGE_SEARCH_PARAM_ERROR,
	IMAGE_SEARCH_ERROR_TASK_TOO_MUCH,                    //�������������  
	IMAGE_SEARCH_ERROR_SOUR_FILE_OPEN,                   //ԭͼƬ�ļ���ʧ��    
	IMAGE_SEARCH_ERROR_REQUEST,                          //����ʧ�ܣ� ����ʧ�����ʹ��msg
	IMAGE_SEARCH_ERROR_REQUEST_EX,                       //����ʧ�ܣ�curl���󣩣� ����ʧ�����ʹ��msg

	//ffmpegת��
	FFMPEG_ERROR = 9000,
};

/**
* ʶ�𷵻ؽ������
* @return
*/
struct ResultData
{
    RESULT_TYPE dwCode = RESULT_TYPE::SUCESS;         //���ؽӿ�, =0:ʶ����ȷ��>=1000,����
    std::string strMsg;                               //���ؾ�����Ϣ,dwCode=0:ʶ����  dwCode>=1000:������Ϣ
    double dwTimep = 0;                               //ʶ��ӿ����õ�ʱ��
};

#define MAX_MSG_LEN 51200

struct ResultDataEx
{
    RESULT_TYPE dwCode;               //���ؽӿ�, =0:ʶ����ȷ��>=1000,����
    char strMsg[MAX_MSG_LEN];         //���ؾ�����Ϣ,dwCode=0:ʶ����  dwCode>=1000:������Ϣ
};

/**
* ����ص�
* @param int ������  0Ϊ��ȷ������Ϊ����
* @param char*  ��һ������Ϊ0ʱΪ���ص����ݣ�>1000Ϊ������Ϣ
* @return
*/
typedef void(NDHYAI_CALL *onResultCallback)(int, char *);
typedef int (NDHYAI_CALL *onResultCallback_1)(const void *usrdata, int result, const void *data, int size);
}