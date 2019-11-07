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

TEST_F(LoggerTest, Init)
{
    auto logger = Logger::Instance();
    logger->SetLevel(Logger::Level::Trace);
    logger->AddSink(std::make_shared<LoggerSinks::Console>());
}
