#pragma once

#include "spdlog/spdlog.h"
#include <string>
#include <mutex>

using namespace std;

namespace logwrapper{

const std::string kLoggerName("101ppt_logger");
const std::string kLoggerInfo("local");
const std::string kLoggerServer("server");
const std::string kLoggerDown("down");
const std::string kLoggerWarn("warn");
const std::string kLoggerError("error");


void Init(const std::wstring& strLogDir);
void Exit();

//
// �����Ϣ�������־���ɱ����
// example: OutputInfo("hello {} {}", "world"�� 13);
//
template <typename Arg1, typename... Args>
inline void OutputInfo(const char* fmt, const Arg1 &arg1, const Args&... args)
{	
	/*if (!guard->Acquire())
		return;*/

	if (spdlog::get(kLoggerInfo.c_str()) != nullptr)
		spdlog::get(kLoggerInfo.c_str())->info(fmt, arg1, args...);

	/*guard->Release();*/
}

//
// ������󼶱����־���ɱ����
// example: OutputError("hello {} {}", "world"�� 13);
//
template <typename Arg1, typename... Args>
inline void OutputError(const char* fmt, const Arg1 &arg1, const Args&... args)
{
	/*if (!guard->Acquire())
		return;*/
	
	if (spdlog::get(kLoggerError.c_str()) != nullptr)
		spdlog::get(kLoggerError.c_str())->error(fmt, arg1, args...);

	/*guard->Release();*/
}

//
// �����Ϣ�������־����������
// example: OutputInfo("hello world");
//
template<typename T>
inline void OutputInfo(const T& msg)
{
	/*if (!guard->Acquire())
		return;*/

	if (spdlog::get(kLoggerInfo.c_str()) != nullptr)
		spdlog::get(kLoggerInfo.c_str())->info(msg);

	/*guard->Release();*/
}

//
// ������󼶱����־����������
// example: OutputError("hello world");
//
template<typename T>
inline void OutputError(const T& msg)
{
	/*if (!guard->Acquire())
		return;*/

	if (spdlog::get(kLoggerError.c_str()) != nullptr)
		spdlog::get(kLoggerError.c_str())->error(msg);

	/*guard->Release();*/
}

//
// �����־��ָ��Ŀ¼���ɱ������
// example: Output("folder", "hello {} {}", "world", 13);
//
//template <typename Arg1, typename... Args>
//inline void Output(const char* area, const char* fmt, const Arg1 &arg1, const Args&... args)
//{
//	if (spdlog::get(area))
//		spdlog::get(area)->info(fmt, arg1, args...);
//}

//
// �����־��ָ��Ŀ¼����������
// example: Output("folder", "hello world");
//
//template<typename T>
//inline void Output(const char* area, const T& msg)
//{
//	if (spdlog::get(area))
//		spdlog::get(area)->info(msg);
//}

//
// ������󼶱����־���ɱ����
// 
//
template <typename Arg1, typename... Args>
inline void OutputServer(const char* fmt, const Arg1 &arg1, const Args&... args)
{
	/*if (!guard->Acquire())
		return;*/

	if (spdlog::get(kLoggerServer.c_str()) != nullptr)
		spdlog::get(kLoggerServer.c_str())->info(fmt, arg1, args...);

	/*guard->Release();*/
}


//
// ������󼶱����־����������
// 
//
template<typename T>
inline void OutputServer(const T& msg)
{
	/*if (!guard->Acquire())
		return;*/

	if (spdlog::get(kLoggerServer.c_str()) != nullptr)
		spdlog::get(kLoggerServer.c_str())->info(msg);

	/*guard->Release();*/
}


//
// ������󼶱����־���ɱ����
// 
//
template <typename Arg1, typename... Args>
inline void OutputDown(const char* fmt, const Arg1 &arg1, const Args&... args)
{
	/*if (!guard->Acquire())
		return;*/

	if (spdlog::get(kLoggerDown.c_str()) != nullptr)
		spdlog::get(kLoggerDown.c_str())->info(fmt, arg1, args...);

	/*guard->Release();*/
}


//
// ������󼶱����־����������
// 
//
template<typename T>
inline void OutputDown(const T& msg)
{
	/*if (!guard->Acquire())
		return;*/

	if (spdlog::get(kLoggerDown.c_str()) != nullptr)
		spdlog::get(kLoggerDown.c_str())->info(msg);

	/*guard->Release();*/
}


} // namespace logwrapper