namespace Logger
{
    namespace Details
    {
        struct Message
        {
            Message()
                : m_ticks(0)
                , m_level(Level::Debug)
                , m_msg()
            {}

            void Set(const uint64_t ticks, const Level level, const std::string& msg)
            {
                m_ticks = ticks;
                m_level = level;
                m_msg = msg;
            }

            uint64_t m_ticks;
            Level m_level;
            std::string m_msg;
        };
    }
}