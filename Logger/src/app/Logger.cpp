#include "Logger.h"

#include <cassert>
#include <fstream>
#include <iomanip>
#include <memory>
#include <sstream>
#include <time.h>

namespace Logger
{
    class Impl
    {
        const std::experimental::filesystem::path path_;
        std::ofstream ofs_;

    public:
        Impl(const std::experimental::filesystem::path& path) :
            path_{path},
            ofs_{path, std::ios_base::app}
        {
            if(!ofs_.good())
            {
                throw std::runtime_error("Failed to open log file: " + path_.u8string());
            }
        }

        void Log(Severity severity, const std::string& msg)
        {
            // TODO(Process & thread ID, escaping msg, rotation, logging of different types).
            ofs_ << MessageTimeStamp(std::chrono::system_clock::now()) << " " << AsString(severity) << " >> " << msg << std::endl;
        }
    };

    std::unique_ptr<Impl> singleton_impl{};

    std::string AsString(Severity severity)
    {
        switch (severity)
        {
        case Severity::Error:
            return "ERR";
        case Severity::Warning:
            return "WRN";
        case Severity::Info:
            return "INF";
        case Severity::Debug:
            return "DBG";
        }
        assert(false);
    }

    std::string MessageTimeStamp(const std::chrono::system_clock::time_point& time_point)
    {
        auto tt{std::chrono::system_clock::to_time_t(time_point)};
        std::tm gmtime{};
        gmtime_r(&tt, &gmtime);

        const std::chrono::duration<double> tse = time_point.time_since_epoch();
        std::chrono::seconds::rep milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(tse).count() % 1000;

        std::ostringstream oss{};
        oss << std::put_time(&gmtime, "%FT%T.") << std::setfill('0') << std::setw(3) << milliseconds << "Z";
        return oss.str();
    }

    std::string FileNameTimeStamp(const std::chrono::system_clock::time_point& time_point)
    {
        return MessageTimeStamp(time_point);
    }

    void Initialise(const std::experimental::filesystem::path& path)
    {
        singleton_impl = std::make_unique<Impl>(path);
    }

    void Log(Severity severity, const std::string& msg)
    {
        if(!singleton_impl)
        {
            return;
        }
        singleton_impl->Log(severity, msg);
    }
}
