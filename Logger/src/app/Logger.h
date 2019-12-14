#pragma once

#include <experimental/filesystem>

namespace Logger
{
    enum class Severity
    {
        Error,
        Warning,
        Info,
        Debug,
    };

    void Initialise(const std::experimental::filesystem::path& path);

    void Log(Severity severity, const std::string& msg);
}

#define LOG_SEVERITY(Severtiy, msg) Logger::Log(Severity, msg)

#define LOG_ERROR(msg) LOG_SEVERITY(Severity::Error, msg)
#define LOG_WARNING(msg) LOG_SEVERITY(Severity::Warning, msg)
#define LOG_INFO(msg) LOG_SEVERITY(Severity::Info, msg)
#define LOG_DEBUG(msg) LOG_SEVERITY(Severity::Debug, msg)
