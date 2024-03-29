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
    Data data{ "ToyProjects.00.data","ToyProjects.01.data","ToyProjects.02.data","ToyProjects.03.data",
               "ToyProjects.04.data","ToyProjects.05.data","ToyProjects.06.data","ToyProjects.07.data",
               "ToyProjects.08.data","ToyProjects.09.data","ToyProjects.10.data"  };
    data.AddDataPath("ToyProjects.11.data");
    auto textShader = data["shaders"].GetShader("Text");
    EXPECT_FALSE(textShader.first.empty());
    EXPECT_FALSE(textShader.second.empty());
    for(const auto& item : data["shaders"])
    {
      SUCCEED() << item.get<0>() << "," << item.get<1>();
    }
}
