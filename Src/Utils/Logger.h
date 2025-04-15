#pragma once

#include <iostream>
#include <cstdio>
#include <windows.h>

class Logger
{
public:
    enum class Level
    {
        Info,
        Warning,
        Error,
        Success,
        Fail
    };
	static void Init();

    // Logging methods
    static void Info(const std::string& message);
    static void Warning(const std::string& message);
    static void Error(const std::string& message);
    static void Success(const std::string& message);
    static void Fail(const std::string& message);

private:
    static void EnableTerminal();

    static void Log(const std::string& prefix, const std::string& message, WORD color);

    static HANDLE s_ConsoleHandle;
    static bool s_Initialized;
};

#define INIT_AND_LOG(fn, ...) \
	if (!fn(__VA_ARGS__)) { \
		Logger::Error("Failed to initialize " #fn); \
		return false; \
	} else { \
		Logger::Info(#fn " initialized successfully."); \
	}
