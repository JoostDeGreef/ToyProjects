#include "gtest/gtest.h"
#include "Logger.h"

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
    void Log(const char* msg) override
    {
      m_messages.emplace_back(msg);
    }
    
    std::vector<std::string> m_messages;
};

TEST_F(LoggerTest, Init)
{
    auto sink = std::make_shared<StoreSink>();
    auto logger = Logger::Logger::Instance();
    logger->SetLevel(Logger::Level::Info);
    logger->AddSink(sink);
    
    logger->Debug("Should not be {}","visible");
    logger->Info("The magic number is {}",42);
    
    EXPECT_EQ(1,sink->m_messages.size());
}

TEST_F(LoggerTest, Levels)
{
    auto sink = std::make_shared<StoreSink>();
    auto logger = Logger::Logger::Instance();
    logger->SetLevel(Logger::Level::Info);
    logger->AddSink(sink);
    
    logger->Trace("Trace");
    logger->Debug("Debug");
    logger->Info("Info");
    logger->Warning("Warning");
    logger->Fatal("Fatal");
    
    EXPECT_EQ(3,sink->m_messages.size());
}
