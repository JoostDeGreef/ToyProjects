#ifdef WIN32
#include "LoggerSinks.h"
#include <Windows.h>

namespace Logger
{
    namespace Sink
    {
        WinDebug::WinDebug(std::shared_ptr<IFormatter> formatter)
            : m_formatter(formatter)
        {
            OutputDebugStringA(m_formatter->Header().c_str());
        }
        WinDebug::~WinDebug()
        {
            OutputDebugStringA(m_formatter->Footer().c_str());
        }
        void WinDebug::Log(const Level level, const uint64_t ticks, const std::string & msg)
        {
            OutputDebugStringA(m_formatter->Format(level, ticks, msg).c_str());
        }
        void WinDebug::Flush()
        {}
    }
}
#endif

