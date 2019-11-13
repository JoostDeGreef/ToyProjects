#include <string>
#include <unordered_map>
#include <algorithm>
#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <chrono>

#include "LoggerSinks.h"

namespace Logger
{
    namespace Sink
    {
        File::File(const boost::filesystem::path& path, std::shared_ptr<IFormatter> formatter)
            : m_formatter(formatter)
            , m_path(path)
            , m_file()
        {
            m_file.open(path.native());
            m_file << m_formatter->Header();
        }
        File::~File()
        {
            m_file << m_formatter->Footer();
            m_file.close();
        }

        void File::Log(const Level level, const uint64_t ticks, const std::string & msg)
        {
            m_file << m_formatter->Format(level,ticks,msg);
        }
        void File::Flush()
        {
            m_file.flush();
        }
    }
}


