#pragma once

#include <vector>
#include <memory>

#define FMT_STRING_ALIAS 1
#include <fmt/format.h>

namespace Logger
{
    enum class Level
    {
        Trace = 1,
        Debug = 2,
        Info = 3,
        Warning = 4,
        Error = 5,
        Fatal = 6
    };

    class ISink
    {
    public:
        virtual void Log(const Level level, const uint64_t ticks, const char* msg) = 0;
    };

    class Logger
    {
    public:

        static const std::shared_ptr<Logger>& Instance(const char* name = "");

        template<typename Sink, typename... Args>
        void AddSink(Args&& ... args);
        void AddSink(std::shared_ptr<ISink> sink);
        void SetLevel(const Level level) { m_level = level; }

        template<typename... Args> void Trace(const char* fmt, Args&& ... args) const;
        template<typename... Args> void Debug(const char* fmt, Args&& ... args) const;
        template<typename... Args> void Info(const char* fmt, Args&& ... args) const;
        template<typename... Args> void Warning(const char* fmt, Args&& ... args) const;
        template<typename... Args> void Error(const char* fmt, Args&& ... args) const;
        template<typename... Args> void Fatal(const char* fmt, Args&& ... args) const;

    private:
        template<typename... Args> void Log(const Level level, const char* fmt, Args&& ... args) const;
        void LogToSinks(const Level level, const std::string& msg) const;

        Level m_level;
        std::vector<std::shared_ptr<ISink>> m_sinks;
    };

    template<typename... Args> void Logger::Trace(const char* fmt, Args&& ... args) const
    {
        Log(Level::Trace, fmt, std::forward<Args>(args)...);
    }
    template<typename... Args> void Logger::Debug(const char* fmt, Args&& ... args) const
    {
        Log(Level::Debug, fmt, std::forward<Args>(args)...);
    }
    template<typename... Args> void Logger::Info(const char* fmt, Args&& ... args) const
    {
        Log(Level::Info, fmt, std::forward<Args>(args)...);
    }
    template<typename... Args> void Logger::Warning(const char* fmt, Args&& ... args) const
    {
        Log(Level::Warning, fmt, std::forward<Args>(args)...);
    }
    template<typename... Args> void Logger::Error(const char* fmt, Args&& ... args) const
    {
        Log(Level::Error, fmt, std::forward<Args>(args)...);
    }
    template<typename... Args> void Logger::Fatal(const char* fmt, Args&& ... args) const
    {
        Log(Level::Fatal, fmt, std::forward<Args>(args)...);
    }

    template<typename... Args> void Logger::Log(const Level level, const char* f, Args&& ... args) const
    {
        if (level >= m_level)
        {
            LogToSinks(level, std::move(fmt::format(f, args...)));
        }
    }

    template<typename Sink, typename... Args> void Logger::AddSink(Args&& ...args)
    {
        auto sink = std::make_shared<Sink>(std::forward<Args>(args)...);
        AddSink(sink);
    }

}; // namespace
