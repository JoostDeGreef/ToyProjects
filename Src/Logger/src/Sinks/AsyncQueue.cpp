#include <string>
#include <unordered_map>
#include <algorithm>
#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <chrono>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <atomic>

#include "LoggerSinks.h"
#include "internal/Message.h"

using namespace Logger::Details;

namespace Logger
{
    namespace Sink
    {
        class AsyncQueueImpl
        {
        public:
            AsyncQueueImpl(std::shared_ptr<ISink>&& sink, uint64_t highwaterMark)
                : m_sink(std::move(sink))
                , m_highwaterMark(highwaterMark)
                , m_messages(highwaterMark*8)
                , m_thread()
                , m_read(0)
                , m_write(0)
            {
                StartThread();
            }
            ~AsyncQueueImpl()
            {
                StopThread();
                WriteAllDirect();
            }
            void StartThread()
            {
                m_thread = std::thread(LogThread, this);
            }
            void StopThread()
            {
                {
                    std::unique_lock<std::mutex> lock(m_state_mutex);
                    m_quit.store(true);
                }
                m_state_cv.notify_one();
                m_thread.join();
            }
            void Log(const Level level, const uint64_t ticks, const std::string& msg)
            {
                m_messages[m_write].Set(ticks, level, msg);
                unsigned int write = m_write++;
                if (write > m_read.load() + m_highwaterMark || write > m_highwaterMark * 7)
                {
                    m_write.store(0);
                    std::unique_lock<std::mutex> lock(m_state_mutex);
                    unsigned int read = m_read.load();
                    unsigned int diff = write - read;
                    if (diff > m_highwaterMark)
                    {
                        diff = m_highwaterMark / 2;
                        read = write - diff;
                        m_messages[read].m_msg = "Logger too slow, skipping messages";
                    }
                    for (unsigned int i = 0; i < diff; ++i)
                    {
                        m_messages[i] = m_messages[i + read];
                    }
                    m_write.store(diff);
                    m_read.store(0);
                }
                m_state_cv.notify_one();
            }
            void WriteAllDirect()
            {
                for (unsigned int i = m_read;i<m_write;++i)
                {
                    WriteMessage(i);
                }
                m_read.store(0);
                m_write.store(0);
            }
            void WriteMessage(const unsigned int i)
            {
                auto& message = m_messages[i];
                m_sink->Log(message.m_level, message.m_ticks, message.m_msg);
            }
            void Flush()
            {
                StopThread();
                WriteAllDirect();
                m_sink->Flush();
                StartThread();
            }
            void LogThreadRunner()
            {
                std::unique_lock<std::mutex> lock(m_state_mutex);
                while (!m_quit.load())
                {
                    m_state_cv.wait(lock, [&]() { return m_write.load() > m_read.load() || m_quit.load(); });
                    while (m_write.load() > m_read.load())
                    {
                        WriteMessage(m_read++);
                    }
                }
            }
            static void LogThread(AsyncQueueImpl* This)
            {
                This->LogThreadRunner();
            }
        private:
            std::shared_ptr<ISink>    m_sink;
            uint64_t                  m_highwaterMark;
            std::vector<Message>      m_messages;
            std::mutex                m_state_mutex;
            std::condition_variable   m_state_cv;
            std::atomic<bool>         m_quit;
            std::thread               m_thread;
            std::atomic<unsigned int> m_read;
            std::atomic<unsigned int> m_write;
        };

        AsyncQueue::AsyncQueue(std::shared_ptr<ISink>&& sink, uint64_t highwaterMark)
            : m_impl(std::make_shared<AsyncQueueImpl>(std::move(sink),highwaterMark))
        {}
        void AsyncQueue::Log(const Level level, const uint64_t ticks, const std::string & msg)
        {
            m_impl->Log(level, ticks, msg);
        }
        void AsyncQueue::Flush()
        {
            m_impl->Flush();
        }
    }
}


