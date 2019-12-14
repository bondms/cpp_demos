#pragma once

#include <chrono>
#include <experimental/filesystem>

namespace Logger
{
    // TODO(Consider moving elsewhere)
    enum class Severity
    {
        Error,
        Warning,
        Info,
        Debug,
    };
    std::string AsString(Severity severity);

    // TODO(Consider moving elsewhere)
    std::string MessageTimeStamp(const std::chrono::system_clock::time_point& time_point);
    std::string FileTimeStamp(const std::chrono::system_clock::time_point& time_point);

    void Initialise(const std::experimental::filesystem::path& path);

    void Log(Severity severity, const std::string& msg);
}

#define LOG_SEVERITY(severity, msg) Logger::Log(severity, msg)

#define LOG_ERROR(msg) LOG_SEVERITY(Logger::Severity::Error, msg)
#define LOG_WARNING(msg) LOG_SEVERITY(Logger::Severity::Warning, msg)
#define LOG_INFO(msg) LOG_SEVERITY(Logger::Severity::Info, msg)
#define LOG_DEBUG(msg) LOG_SEVERITY(Logger::Severity::Debug, msg)
