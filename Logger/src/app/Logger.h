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
    const char* AsString(Severity severity) noexcept;

    // TODO(Consider moving elsewhere)
    std::string MessageTimeStamp(const std::chrono::system_clock::time_point& time_point) noexcept;
    std::string FileNameTimeStamp(const std::chrono::system_clock::time_point& time_point);

    // Non thread-safe.
    // Ensure logger is initialised before spawning child threads that log.
    void Initialise(const std::experimental::filesystem::path& path);

    // Thread safe.
    void Log(Severity severity, const std::string& msg) noexcept;

    enum class ErrorReporting
    {
        ignoreError,
        throwOnError
    };

    // Thread safe TOOD(Confirm this).
    // The log file will automatically be flushed when the application ends.
    // The Flush method may be used to flush the file early and/or to check for flush failure;
    // an exception can be thrown on failure.
    void Flush(ErrorReporting errorReporting);

    // Non thread-safe.
    // Ensure logger is only closed after any child threads have finished logging.
    // The log file will automatically be closed when the application ends.
    // The Close method may be used to close the file early and/or to check for close failure;
    // an exception can be thrown on failure.
    // After Close()ing, another call to Initialise() is require if more logging is required.
    void Close(ErrorReporting errorReporting);
}

#define LOG_SEVERITY(severity, msg) Logger::Log(severity, msg)

#define LOG_ERROR(msg) LOG_SEVERITY(Logger::Severity::Error, msg)
#define LOG_WARNING(msg) LOG_SEVERITY(Logger::Severity::Warning, msg)
#define LOG_INFO(msg) LOG_SEVERITY(Logger::Severity::Info, msg)
#define LOG_DEBUG(msg) LOG_SEVERITY(Logger::Severity::Debug, msg)
