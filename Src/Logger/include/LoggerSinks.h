#pragma once

#include <iostream>
#include <boost/filesystem.hpp>

#include "Logger.h"

namespace Logger
{
    namespace Sink
    {
        // Write directly to a stream.
        // Created with std::clog, std::cout and std::cerr in mind, but can be any persistent std::ostream
        class Console final : public ISink
        {
        public:
            Console(std::ostream& stream = std::clog)
                : m_stream(stream)
            {}

            void Log(const Level level, const uint64_t ticks, const char* msg) override;
        private:
            std::ostream& m_stream;
        };


        // Write to a file.
        // Files can be (too) slow, see AsyncQueue for solution
        class File final : public ISink
        {
        public:
            File(const boost::filesystem::path& path)
                : m_path(path)
            {}

            void Log(const Level level, const uint64_t ticks, const char* msg) override;
        private:
            boost::filesystem::path m_path;
        };


        // Process the messages asynchronous    
        // When the worker sink can not keep up, dispose of messages
        class AsyncQueue final : public ISink
        {
        public:
            AsyncQueue(std::shared_ptr<ISink>&& sink)
                : m_sink(std::move(sink))
            {}

            void Log(const Level level, const uint64_t ticks, const char* msg) override;
        private:
            std::shared_ptr<ISink> m_sink;
        };
    } // namespace
} // namespace

