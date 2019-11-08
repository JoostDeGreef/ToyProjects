#include <string>
#include <unordered_map>
#include <algorithm>
#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <chrono>
using namespace std;

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
    uint64_t ticks = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    for (const auto& sink : m_sinks)
    {
        sink->Log(level,ticks,msg.c_str());
    }
  }
}


