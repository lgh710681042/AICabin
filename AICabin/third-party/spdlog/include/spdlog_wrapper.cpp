#include "stdafx.h"
#include <ctime>
#include <chrono>
#include <sstream>
#include <iomanip>
#include <tchar.h>
#include "spdlog_wrapper.hpp" // spdlog

using std::chrono::system_clock;

namespace logwrapper{

bool InitLog(const char* area, const std::wstring& strLogDir, spdlog::level::level_enum log_level)
{
	try {
		system_clock::time_point tp = system_clock::now();

		time_t raw_time = system_clock::to_time_t(tp);

		// tm*使用完后不用delete，因为tm*是由localtime创建的，并且每个线程中会有一个
		struct tm timeinfo;

		localtime_s(&timeinfo, &raw_time);

		std::wstringstream ss;
		ss << std::put_time(&timeinfo, _T("%Y%m%d"));

		wstring strFileName = ss.str() + _T(".log");
		std::wstring logPath = strLogDir + _T("\\") + strFileName;
		
		std::shared_ptr<spdlog::logger> sptrLoger = spdlog::basic_logger_mt(area, logPath);
		if (nullptr == sptrLoger) return false;
		sptrLoger->flush_on(log_level);
		sptrLoger->set_pattern("%T.%e %t %l %v");
	}
	catch (...) {
		// todo
		return false;
	}

	return true;
}

void Init(const std::wstring& strLogDir)
{
	InitLog(kLoggerInfo.c_str(), strLogDir, spdlog::level::info);
}

void Exit()
{
	spdlog::drop_all();
}

} // namespace logwrapper