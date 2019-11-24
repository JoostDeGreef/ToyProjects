#pragma once

#include <iostream>
#include <boost/filesystem.hpp>

#include "Logger.h"

namespace Logger
{
    class IFormatter
    {
    public:
        virtual std::string Header() = 0;
        virtual std::string Footer() = 0;
        virtual std::string Format(const Level level, const uint64_t ticks, const std::string& msg) = 0;
    };

    namespace Formatter
    {
        class Simple : public IFormatter
        {
            std::string Header() override
            {
                return "";
            }
            std::string Footer() override
            {
                return "";
            }
            std::string Format(const Level level, const uint64_t ticks, const std::string& msg) override
            {
                std::string res;
                switch (level)
                {
                case Level::Trace:   res = "[ TRACE    ] "; break;
                case Level::Debug:   res = "[ DEBUG    ] "; break;
                case Level::Info:    res = "[ INFO     ] "; break;
                case Level::Warning: res = "[ WARNING  ] "; break;
                case Level::Error:   res = "[ ERROR    ] "; break;
                case Level::Fatal:   res = "[ FATAL    ] "; break;
                }
                return res + msg + "\n";
            }
        };
    }

    namespace Sink
    {
        // Write directly to a stream.
        // Created with std::clog, std::cout and std::cerr in mind, but can be any persistent std::ostream
        class Console final : public ISink
        {
        public:
            Console(std::ostream& stream = std::clog, std::shared_ptr<IFormatter> formatter = std::make_shared<Formatter::Simple>());
            ~Console();

            void Log(const Level level, const uint64_t ticks, const std::string & msg) override;
            void Flush() override;
        private:
            std::shared_ptr<IFormatter> m_formatter;
            std::ostream& m_stream;
        };


        // while running under the visual studio ide, or windbg, send
        // debug messages
#ifdef WIN32
        class WinDebug final : public ISink
        {
        public:
            WinDebug(std::shared_ptr<IFormatter> formatter = std::make_shared<Formatter::Simple>());
            ~WinDebug();

            void Log(const Level level, const uint64_t ticks, const std::string& msg) override;
            void Flush() override;
        private:
            std::shared_ptr<IFormatter> m_formatter;
        };
#endif

        // Write to a file.
        // Files can be (too) slow, see AsyncQueue for a solution
        class File final : public ISink
        {
        public:
            File(const boost::filesystem::path& path, std::shared_ptr<IFormatter> formatter = std::make_shared<Formatter::Simple>());
            ~File();

            void Log(const Level level, const uint64_t ticks, const std::string & msg) override;
            void Flush() override;
        private:
            std::shared_ptr<IFormatter> m_formatter;
            boost::filesystem::path m_path;
            std::ofstream m_file;
        };


        // Process the messages asynchronous
        // When the worker sink can not keep up, dispose of messages
        class AsyncQueueImpl;
        class AsyncQueue final : public ISink
        {
        public:
            AsyncQueue(std::shared_ptr<ISink>&& sink, uint64_t highwaterMark = 256);

            void Log(const Level level, const uint64_t ticks, const std::string & msg) override;
            void Flush() override;
        private:
            std::shared_ptr<AsyncQueueImpl> m_impl;
        };
                
    } // namespace
} // namespace

