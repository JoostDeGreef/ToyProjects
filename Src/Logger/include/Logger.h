#pragma once

#include <vector>
#include <memory>
#include <boost/filesystem.hpp>
#include <boost/format.hpp>

class ILoggerSink
{
public:
    virtual void Log(const char* msg) const = 0;
};

namespace LoggerSinks
{
    class Console final : public ILoggerSink
    {
    public:
        void Log(const char* msg) const override;
    };

    class File final : public ILoggerSink
    {
    public:
        void Log(const char* msg) const override;
    };
}

class Logger
{
public:
    enum class Level
    {
        Trace    = 1,
        Debug    = 2,
        Info     = 3,
        Warning  = 4,
        Error    = 5,
        Fatal    = 6
    };

    static std::shared_ptr<Logger> Instance();

    void AddSink(std::shared_ptr<ILoggerSink> sink);
    void SetLevel(const Level level) { m_level = level; }

    template<typename... Args> void Trace(const char * fmt,Args&& ... args) const;
    template<typename... Args> void Debug(const char* fmt, Args&& ... args) const;
    template<typename... Args> void Info(const char* fmt, Args&& ... args) const;
    template<typename... Args> void Warning(const char* fmt, Args&& ... args) const;
    template<typename... Args> void Error(const char* fmt, Args&& ... args) const;
    template<typename... Args> void Fatal(const char* fmt, Args&& ... args) const;

private:
    template<typename... Args> void Log(const Level level,const char* fmt, Args&& ... args) const;
    void Log(const char * msg) const;

    Level m_level;
    std::vector<std::shared_ptr<ILoggerSink>> m_sinks;
};

template<typename... Args> void Logger::Trace(const char* fmt, Args&& ... args) const
{
    Log(Level::Trace, fmt, std::forward<Args>(args...));
}
template<typename... Args> void Logger::Debug(const char* fmt, Args&& ... args) const
{
    Log(Level::Debug, fmt, std::forward<Args>(args...));
}
template<typename... Args> void Logger::Info(const char* fmt, Args&& ... args) const
{
    Log(Level::Info, fmt, std::forward<Args>(args...));
}
template<typename... Args> void Logger::Warning(const char* fmt, Args&& ... args) const
{
    Log(Level::Warning, fmt, std::forward<Args>(args...));
}
template<typename... Args> void Logger::Error(const char* fmt, Args&& ... args) const
{
    Log(Level::Error, fmt, std::forward<Args>(args...));
}
template<typename... Args> void Logger::Fatal(const char* fmt, Args&& ... args) const
{
    Log(Level::Fatal, fmt, std::forward<Args>(args...));
}

template<typename... Args> void Logger::Log(const Level level, const char* fmt, Args&& ... args) const
{
    if (level >= m_level)
    {
        boost::format f(fmt);
        (f % ... % std::forward<Args>(args));
        Log(f.str().c_str());
    }
}

