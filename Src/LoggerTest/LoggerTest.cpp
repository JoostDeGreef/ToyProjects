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

class StoreSink final : public Logger::ISink
{
public:
    void Log(const Logger::Level level,const uint64_t ticks,const char* msg) override
    {
      m_messages.emplace_back(msg);
    }
    
    std::vector<std::string> m_messages;
};

TEST_F(LoggerTest, Console)
{
    auto logger = Logger::Logger::Instance("Console");
    logger->SetLevel(Logger::Level::Info);
    logger->AddSink<Logger::Sink::Console>();
    
    logger->Debug("Is this {}?","visible");
    logger->Info("The magic number is {}",42);
}

TEST_F(LoggerTest, Format)
{
    auto sink = std::make_shared<StoreSink>();
    auto logger = Logger::Logger::Instance("Init");
    logger->SetLevel(Logger::Level::Info);
    logger->AddSink(sink);
    
    logger->Debug("Should not be {}","visible");
    logger->Info("The magic number is {}",42);
    
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
    
    EXPECT_EQ(4,sink->m_messages.size());
}


