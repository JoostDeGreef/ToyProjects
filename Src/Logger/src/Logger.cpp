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

#include <boost/lockfree/queue.hpp>

#include "Logger.h"

namespace Logger
{
  const std::shared_ptr<Logger> & Logger::Instance(const char* name)
  {
    static std::map<std::string,std::shared_ptr<Logger>> loggers;
    if(!loggers[name])
    {
      auto logger = std::make_shared<Logger>();
      logger->SetLevel(Level::Info);
      loggers[name] = logger;
    }
    return loggers[name];
  }

  void Logger::AddSink(std::shared_ptr<ISink> sink)
  {
    m_sinks.emplace_back(sink);
  }

  void Logger::LogToSinks(const Level level, const std::string & msg) const
  {
    const int bufferSize = 1024;
    struct Message
    {
        Message()
        {
           m_size = 128;
           m_msg = new char[m_size];
        }
        ~Message()
        {
          delete [] m_msg;
        }
        void Resize(const size_t s)
        {
          delete m_msg;
          m_size = s;
          m_msg = new char[m_size];
        }
    
        uint64_t m_ticks;
        Level m_level;
        int m_size;
        char * m_msg;
    };
    class MessageStore
    {
    public:
      MessageStore()
      {
        AddBlock();
      }
      ~MessageStore()
      {}
      Message* AllocMessage(const Level level, const std::string & msg)
      {
        uint64_t ticks = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
        Message* message;
        while(!m_messagePtrStore.pop(message))
        {
          AddBlock();
        }
        message->m_ticks = ticks;
        message->m_level = level;
        if(message->m_size < msg.size()+1)
        {
          message->resize(msg.size()+1);
        }
        memcpy(message.m_msg,msg.c_str(),msg.size()+1);
        return message;
      }
      void FreeMessage(Message* msg)
      {
        messagePtrStore.push(msg);
      }
    private:
      std::list<std::vector<Message>> m_messageStore;
      boost::lockfree::queue<Message*> m_messagePtrStore;
      std::mutex m_mutex;
      
      void AddBlock()
      {
        std::lock_guard<std::mutex> lock(m_mutex);
        std::vector<Message> block;
        block.resize(128);
        for(auto & message: block)
        {
           messagePtrStore.push(&message);
        }
        m_messageStore.push_back(std::move(block));
      }
    };
    static MessageStore messageStore;
    static boost::lockfree::queue<Message*> messages(bufferSize);
    messages.push(Message{ticks,level,AllocString(msg)});

    // todo: add worker thread around this

    messages.consume_all([&](Message* message)
        {
            for (const auto& sink : m_sinks)
            {
                sink->Log(message->m_level,message->m_ticks,message->m_msg);
            }
            messageStore.FreeMessage(message);
        });
  }
}


