#include "Logger.h"

HANDLE Logger::s_ConsoleHandle = nullptr;
bool Logger::s_Initialized = false;

void Logger::Init()
{
    if (!s_Initialized)
        EnableTerminal();
}

void Logger::EnableTerminal()
{
#ifdef NDEBUG
    return;
#endif
    // Create a new console window if one doesn't exist
    if (!AllocConsole()) return;

    // Redirect standard output to the console
    FILE* dummy;
    freopen_s(&dummy, "CONOUT$", "w", stdout);
    freopen_s(&dummy, "CONOUT$", "w", stderr);
    freopen_s(&dummy, "CONIN$", "r", stdin);

    s_ConsoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    s_Initialized = true;
}

// Color codes:
// https://learn.microsoft.com/en-us/windows/console/console-screen-buffers#character-attributes
void Logger::Log(const std::string& prefix, const std::string& message, WORD color)
{
    if (!s_Initialized)
        EnableTerminal();

    SetConsoleTextAttribute(s_ConsoleHandle, color);
    std::cout << "[" << prefix << "] " << message << std::endl;

    // Reset to default
    SetConsoleTextAttribute(s_ConsoleHandle, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
}

void Logger::Info(const std::string& message)
{
    Log("INFO", message, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
}

void Logger::Warning(const std::string& message)
{
    Log("WARNING", message, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
}

void Logger::Error(const std::string& message)
{
    Log("ERROR", message, FOREGROUND_RED | FOREGROUND_INTENSITY);
}

void Logger::Success(const std::string& message)
{
    Log("SUCCESS", message, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
}

void Logger::Fail(const std::string& message)
{
    Log("FAIL", message, FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
}
