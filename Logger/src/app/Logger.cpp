#include "Logger.h"

#include <fstream>
#include <memory>

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

        void Log(Severity /*severity*/, const std::string& /*msg*/)
        {
            // TODO
        }
    };

    std::unique_ptr<Impl> singleton_impl{};

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

