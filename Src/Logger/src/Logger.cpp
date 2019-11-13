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

#include "Logger.h"

namespace Logger
{
  namespace 
  {
    struct Message
    {
        void Set(const uint64_t ticks, const Level level, std::string && msg)
        {
          m_ticks = ticks;
          m_level = level;
          m_msg = std::move(msg);
        }
    
        uint64_t m_ticks;
        Level m_level;
        std::string m_msg;
    };
  }
  
  class LoggerCore
  {
    static const unsigned int buckets = 8;
  public:
    LoggerCore()
      : m_count(0)
      , m_writer_index(0)
      , m_quit(false)
      , m_thread()
    {
      for(int i=0;i<buckets;++i)
      {
        m_state[i].store(0);
      }
      StartThread();
    }
    ~LoggerCore()
    {
      StopThread();
    }
    
    void StartThread()
    {
      m_thread = std::thread(LoggerThread,this);
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
    
    void Flush()
    {
      StopThread();
      for (const auto& sink : m_sinks)
      {
        sink->Flush();
      }    
      StartThread();
    }
    
    void LogToSinks(const uint64_t ticks, const Level level, std::string && msg)
    {
      for(;;)
      {
        int index = (m_writer_index++) % buckets;
        int state = m_state[index]++;
        if(state == 0)
        {
          m_messages[index].Set(ticks,level,std::move(msg));
          m_count++;
          m_state[index].store(10000);
          m_state_cv.notify_one();
          return;
        }
        else
        {
          --m_state[index];
        }        
      }    
    }

    void LoggerThreadRunner()
    {
      std::unique_lock<std::mutex> lock(m_state_mutex);
      int index = 0;
      while(!m_quit.load() || m_count.load()>0)
      {
        if(m_count.load()<=0 && !m_quit.load())
        {
          m_state_cv.wait(lock,[&](){ return m_count.load()>0 || m_quit.load();});
        }
        while(m_count.load() > 0)
        {
          index = (index+1) % buckets;
          if(m_state[index].load() >= 10000)
          {
            auto & message = m_messages[index];
            for (const auto& sink : m_sinks)
            {
              sink->Log(message.m_level,message.m_ticks,message.m_msg.c_str());
            }
            --m_count;
            m_state[index] -= 10000;
          }
        }
      }
    }    
    static void LoggerThread(LoggerCore *This)
    {
      This->LoggerThreadRunner();
    }
    
    std::atomic<int>             m_count;
    std::atomic<unsigned int>    m_writer_index;
    std::atomic<bool>            m_quit;
    std::atomic<int>             m_state[buckets];
    std::array<Message,buckets>  m_messages;
    std::vector<std::shared_ptr<ISink>> m_sinks;
    std::mutex                   m_state_mutex;
    std::condition_variable      m_state_cv;
    std::thread                  m_thread;
  };

  Logger::Logger(const Level level)
  {
    m_core = new LoggerCore;
  }

  Logger::~Logger()
  {
    delete m_core;
  }

  std::shared_ptr<Logger> Logger::Instance(const char* name,const Level level)
  {
    static std::map<std::string,std::weak_ptr<Logger>> loggers;    
    auto ptr = loggers[name].lock();
    if(!ptr)
    {
      ptr = std::make_shared<Logger>(level);
      loggers[name] = ptr;
    }
    return ptr;
  }

  void Logger::AddSink(std::shared_ptr<ISink> sink)
  {
    m_core->m_sinks.emplace_back(sink);
  }

  void Logger::LogToSinks(const Level level, std::string && msg) const
  {
    uint64_t ticks = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    m_core->LogToSinks(ticks, level, std::move(msg));
  }
  
  void Logger::Flush()
  {
    m_core->Flush();
  }
}


