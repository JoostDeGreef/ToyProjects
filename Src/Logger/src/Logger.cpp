#include <string>
#include <unordered_map>
#include <algorithm>
#include <iostream>
#include <vector>
#include <map>
#include <set>
using namespace std;

#include <boost/dll.hpp>
#include <boost/optional.hpp>

#include <spdlog/spdlog.h>

#include "Logger.h"

namespace LoggerSinks
{
    void Console::Log(const char* msg) const
    {
        cout << msg << endl;
    }

    void File::Log(const char* msg) const
    {

    }
}

std::shared_ptr<Logger> Logger::Instance()
{
    static auto logger = make_shared<Logger>();
    return logger;
}

void Logger::AddSink(std::shared_ptr<ILoggerSink> sink)
{
    m_sinks.emplace_back(sink);
}

void Logger::Log(const char* msg) const
{
    for (const auto& sink : m_sinks)
    {
        sink->Log(msg);
    }
}
