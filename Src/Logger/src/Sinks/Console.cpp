#include <iostream>

#include "LoggerSinks.h"

namespace Logger
{
    namespace Sink
    {
        void Console::Log(const Level level, const uint64_t ticks, const char* msg)
        {
            switch (level)
            {
            case Level::Trace:   m_stream << "[ Trace    ] "; break;
            case Level::Debug:   m_stream << "[ Debug    ] "; break;
            case Level::Info:    m_stream << "[ Info     ] "; break;
            case Level::Warning: m_stream << "[ Warning  ] "; break;
            case Level::Error:   m_stream << "[ Error    ] "; break;
            case Level::Fatal:   m_stream << "[ Fatal    ] "; break;
            }
            m_stream << msg << "\n";
        }
        void Console::Flush()
        {
           m_stream.flush();
        }
    }
}


