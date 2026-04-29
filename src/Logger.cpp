#include "Logger.h"
#include <iostream>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>

std::vector<LogEntry> Logger::messages;

std::string CurrentDateTimeToString()
{
    auto now = std::chrono::system_clock::now();
    std::time_t now_time = std::chrono::system_clock::to_time_t(now);
    std::tm localTime{};

#ifdef _WIN32
    localtime_s(&localTime, &now_time);
#else
    localtime_r(&now_time, &localTime);
#endif

    std::ostringstream oss;
    oss << std::put_time(&localTime, "%d/%B/%Y %I:%M:%S %p");
    return oss.str();
}

void Logger::Log(const std::string &message)
{
    LogEntry LogEntry;

    LogEntry.type = INFO;
    LogEntry.message = "[LOG | " + CurrentDateTimeToString() + "] - " + message;
    std::cout << "\x1B[32m" << LogEntry.message << "\033[0m" << std::endl; // Green

    messages.push_back(LogEntry);
}

void Logger::Warn(const std::string &message)
{
    LogEntry LogEntry;

    LogEntry.type = WARNING;
    LogEntry.message = "[WARNING | " + CurrentDateTimeToString() + "] - " + message;
    std::cout << "\x1B[38;2;255;255;85m" << LogEntry.message << "\033[0m" << std::endl; // yellow

    messages.push_back(LogEntry);
}

void Logger::Err(const std::string &message)
{
    LogEntry LogEntry;

    LogEntry.type = ERROR;
    LogEntry.message = "[ERROR | " + CurrentDateTimeToString() + "] - " + message;
    std::cerr << "\x1B[38;2;255;50;50m" << LogEntry.message << "\033[0m" << std::endl; // Red

    messages.push_back(LogEntry);
}
