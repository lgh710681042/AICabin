// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             //  �� Windows ͷ�ļ����ų�����ʹ�õ���Ϣ
// Windows ͷ�ļ�: 
#include <windows.h>

// C ����ʱͷ�ļ�
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <mmsystem.h>
#include <atlbase.h>
#include <atlcom.h>
#include <shobjidl.h>

// TODO:  �ڴ˴����ó�����Ҫ������ͷ�ļ�
#include "DirectUI.h"
#include "NDHttp/curl/curl.h"
#include "spdlog_wrapper.hpp"

//����DUI�������õ�stl���ģ���࣬�򵥽��õ����ྯ��
#pragma warning( disable: 4251 )

#ifdef _DEBUG
#pragma comment( lib,"DirectUI_ud.lib" ) 
#pragma comment(lib, "NDHttp_ud.lib")
#else
#pragma comment( lib,"DirectUI_u.lib" )
#pragma comment(lib, "NDHttp_u.lib")
#endif

#pragma comment(lib, "WINMM.LIB")