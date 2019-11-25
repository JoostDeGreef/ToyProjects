#include <vector>
#include <thread>
#include <chrono>
#include "gtest/gtest.h"
#include "Logger.h"
#include "LoggerSinks.h"

using namespace std;
using namespace testing;

class LoggerTest : public Test
{
protected:

  virtual void SetUp()
  {
  }

  virtual void TearDown()
  {
  }
};

class BlockedCounterSink final : public Logger::ISink
{
public:
    BlockedCounterSink()
      : m_count(0)
      , m_mutex()
      , m_lock(m_mutex, std::defer_lock)
    {}
    void Log(const Logger::Level level,const uint64_t ticks,const std::string & msg) override
    {
        m_count++;
    }
    void Flush() override
    {}
    void Lock()
    {
        m_lock.lock();
    }
    void Unlock()
    {
        m_lock.unlock();
    }
    unsigned int Count() const
    {
        return m_count;
    }
private:
    unsigned int m_count;
    std::mutex m_mutex;
    std::unique_lock<std::mutex> m_lock;
};

class StoreSink final : public Logger::ISink
{
public:
    void Log(const Logger::Level level,const uint64_t ticks,const std::string & msg) override
    {
      m_messages.emplace_back(msg);
    }
    void Flush() override
    {}
    
    std::vector<std::string> m_messages;
};

TEST_F(LoggerTest, Console)
{
    auto logger = Logger::Logger::Instance("Console");
    logger->SetLevel(Logger::Level::Info);
    logger->AddSink<Logger::Sink::Console>();
    
    logger->Debug("Is this {}?","visible");
    logger->Info("The magic number is {}",42);
    
    logger.reset();
}

#ifdef WIN32
TEST_F(LoggerTest, WinDBG)
{
    auto logger = Logger::Logger::Instance("WinDBG");
    logger->SetLevel(Logger::Level::Info);
    logger->AddSink<Logger::Sink::WinDebug>();

    logger->Info("See if this line appear in the debugger.");

    logger.reset();
}
#endif

TEST_F(LoggerTest, File)
{
    auto file = boost::filesystem::unique_path();
    auto logger = Logger::Logger::Instance("File");
    logger->SetLevel(Logger::Level::Info);
    logger->AddSink<Logger::Sink::File>(file);

    logger->Debug("Is this {}?", "visible");
    logger->Info("The magic number is {}", 42);

    logger.reset();

    std::ifstream f(file.c_str());
    int n = std::count(std::istreambuf_iterator<char>(f), std::istreambuf_iterator<char>(), '\n');
    f.close();

    boost::filesystem::remove(file);

    EXPECT_EQ(1, n);
}

TEST_F(LoggerTest, AsyncQueue)
{
    using namespace std::chrono_literals;
    auto counter = std::make_shared<BlockedCounterSink>();
    auto logger = Logger::Logger::Instance("Blocked");
    logger->SetLevel(Logger::Level::Info);
    logger->AddSink<Logger::Sink::AsyncQueue>(counter,16);
    counter->Lock();
    
    for(unsigned int i=0;i<1000;++i)
    {
        logger->Info("Logging from a loop: {}", i);
        std::this_thread::yield();
    }
    std::this_thread::sleep_for(2s);
    counter->Unlock();
    logger.reset();

    EXPECT_GT(1000, counter->Count());
    EXPECT_LE(8, counter->Count());
}

TEST_F(LoggerTest, Format)
{
    auto sink = std::make_shared<StoreSink>();
    auto logger = Logger::Logger::Instance("Init");
    logger->SetLevel(Logger::Level::Info);
    logger->AddSink(sink);
    
    logger->Debug("Should not be {}","visible");
    logger->Info("The magic number is {}",42);
    
    logger.reset();
    
    EXPECT_EQ("The magic number is 42",sink->m_messages.front());
}

TEST_F(LoggerTest, Levels)
{
    auto sink = std::make_shared<StoreSink>();
    auto logger = Logger::Logger::Instance("Levels");
    logger->SetLevel(Logger::Level::Info);
    logger->AddSink(sink);
    
    logger->Trace("Trace");
    logger->Debug("Debug");
    logger->Info("Info");
    logger->Warning("Warning");
    logger->Error("Error");
    logger->Fatal("Fatal");
   
    logger->Flush();
    
    EXPECT_EQ(4,sink->m_messages.size());
   
    logger.reset();
}

TEST_F(LoggerTest, StressTest)
{
    auto sink = std::make_shared<StoreSink>();
    auto logger = Logger::Logger::Instance("Levels");
    logger->AddSink(sink);

    auto ProduceLogs = [&]() 
    {
        for (int i = 0; i < 1000; ++i)
        {
            logger->Warning("test {}",i);
//            std::this_thread::yield();
        }
    };

    std::vector<std::thread> threads;
    for (int i = 0; i < 100; ++i)
    {
        threads.emplace_back(ProduceLogs);
    }

    for (auto & t : threads)
    {
        t.join();
    }

    logger->Flush();

    EXPECT_EQ(100*1000, sink->m_messages.size());

    logger.reset();
}


