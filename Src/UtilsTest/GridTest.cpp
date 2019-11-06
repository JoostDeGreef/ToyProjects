#include <memory>
#include <random>
#include <vector>
#include <map>
#include <cassert>
#include <sstream>
#include <iostream>
#include <sstream>

#include "gtest/gtest.h"
#include "grid.h"

using namespace std;
using namespace testing;

class GridTest : public Test
{
protected:

    virtual void SetUp()
    {
    }

    virtual void TearDown()
    {
    }

};

TEST_F(GridTest, Size)
{
    grid<int, long> g1 = { {1,11},{2,22},{3,33},{2,222},{5,55},{23,23},{23,64} };
    const auto i0 = g1.size();
    ASSERT_EQ(7, i0);
}

TEST_F(GridTest, CopyConstructor)
{
    grid<int, long> g0 = { {1,11},{2,22},{3,33},{2,222},{5,55},{23,23},{23,64} };
    grid<int, long> g1(g0);
    const auto i0 = g1.size();
    ASSERT_EQ(7, i0);
}

TEST_F(GridTest, MoveConstructor)
{
    grid<int, long> g0 = { {1,11},{2,22},{3,33},{2,222},{5,55},{23,23},{23,64} };
    grid<int, long> g1(std::move(g0));
    const auto i0 = g1.size();
    ASSERT_EQ(7, i0);
}

TEST_F(GridTest, Clear)
{
    grid<int, long> g1 = { {1,11},{2,22},{3,33},{2,222} };
    g1.clear();
    const auto s0 = g1.size();
    ASSERT_EQ(0, s0);
}

TEST_F(GridTest, Emplace)
{
    grid<int, long> g1 = { {1,11},{2,22},{3,33},{2,222} };
    g1.emplace(4, 44);
    const auto s0 = g1.size();
    ASSERT_EQ(5, s0);
}

TEST_F(GridTest, Get)
{
    grid<int, long> g1 = { {1,11},{2,22},{3,33},{2,222} };
    const auto i0 = g1.get<0, 1>(3);
    ASSERT_EQ(33, i0);
}

TEST_F(GridTest, Find)
{
    grid<int, int> g0;
    grid<int, long> g1 = { {1,11},{2,22},{3,33} };
    SUCCEED() << g1;
    const int i0 = g1.find<0>(2)->get<1>();
    const int i1 = g1.find<1>(22)->get<1>();
    ASSERT_EQ(i0, i1);
    const int i2 = g1.find<0,1>(2,22)->get<1>();
    ASSERT_EQ(i0, i2);
}

TEST_F(GridTest, ToString)
{
    grid<int, long> g1 = { {1,11},{1,11} };
    std::stringstream ss;
    ss << g1;
    std::string s = ss.str();
    ASSERT_EQ("{{1,11},{1,11}}", s); // sorted by hash, multiple different values will fail when using different compilers
}

TEST_F(GridTest, Erase)
{
    grid<int, long> g1 = { {1,11},{2,22},{3,33},{2,222} };
    ASSERT_TRUE(g1.erase<0>(3));
    auto s0 = g1.size();
    ASSERT_EQ(3, s0);
    auto i0 = g1.find<0>(2);
    g1.erase<0>(i0);
    s0 = g1.size();
    ASSERT_EQ(2, s0);
    ASSERT_FALSE(g1.erase<0>(3));
}

TEST_F(GridTest, EraseAll)
{
    grid<int, long> g1 = { {1,11},{2,22},{3,33},{2,222} };
    auto res = g1.erase_all<0>(2);
    EXPECT_EQ(2, res);
    auto s0 = g1.size();
    EXPECT_EQ(2, s0);
}

TEST_F(GridTest, FindAll)
{
    grid<int, long> g1 = { {1,11},{2,22},{3,33},{2,222} };
    const auto i0 = g1.find_all<0>(2);
    const auto dist = std::distance(std::get<0>(i0), std::get<1>(i0));
    ASSERT_EQ(2, dist);
}


