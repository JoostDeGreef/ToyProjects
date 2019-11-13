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
        void File::Log(const Level level, const uint64_t ticks, const std::string & msg)
        {

        }
        void File::Flush()
        {
        
        }
    }
}


