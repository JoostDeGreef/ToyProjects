#include "gtest/gtest.h"
#include "Data.h"

using namespace std;
using namespace testing;

class DataTest : public Test
{
protected:

	virtual void SetUp()
	{
	}

	virtual void TearDown()
	{
	}
};

TEST_F(DataTest, Init)
{
    Data data("ToyProject.data");
}
