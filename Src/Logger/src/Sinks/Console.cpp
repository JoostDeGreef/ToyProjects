#include <iostream>

#include "LoggerSinks.h"

namespace Logger
{
    namespace Sink
    {
        Console::Console(std::ostream& stream, std::shared_ptr<IFormatter> formatter)
            : m_formatter(formatter)
            , m_stream(stream)
        {
            m_stream << m_formatter->Header();
        }
        Console::~Console()
        {
            m_stream << m_formatter->Footer();
        }
        void Console::Log(const Level level, const uint64_t ticks, const std::string & msg)
        {
            m_stream << m_formatter->Format(level, ticks, msg);
        }
        void Console::Flush()
        {
           m_stream.flush();
        }
    }
}


