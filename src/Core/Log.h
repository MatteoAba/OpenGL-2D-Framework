#pragma once

#include "../oglph.h"

#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>

class Log
{
public:
	static void Init();
	inline static std::shared_ptr<spdlog::logger>& GetLogger() { return s_Logger; }

private:
	// il logger � statico e condiviso, per poterlo richiamare da application
	static std::shared_ptr<spdlog::logger> s_Logger;
};

// MACRO per utilizzare il logger
#define LOG_TRACE(...)         Log::GetLogger()->trace(__VA_ARGS__)
#define LOG_INFO(...)          Log::GetLogger()->info(__VA_ARGS__)
#define LOG_WARN(...)          Log::GetLogger()->warn(__VA_ARGS__)
#define LOG_ERROR(...)         Log::GetLogger()->error(__VA_ARGS__)
#define LOG_CRITICAL(...)      Log::GetLogger()->critical(__VA_ARGS__)