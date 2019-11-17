#include "gtest/gtest.h"

#include "MathCore.h"

using namespace std;
using namespace testing;

class MatrixFixedTest : public Test
{
protected:
    virtual void SetUp()
    {
    }

    virtual void TearDown()
    {
    }

    const double eps = 0.00000001;
};

TEST_F(MatrixFixedTest, Constructor)
{
    TMatrixFixed<int,2,2> m;
    EXPECT_EQ(0, m(0, 0));
}

TEST_F(MatrixFixedTest, Copy)
{
    TMatrixFixed<int,2,2> m1;
    m1.Fill(1);
    TMatrixFixed<int,2,2> m2(m1);
    EXPECT_EQ(1, m2(0, 1));
}

TEST_F(MatrixFixedTest, Clear)
{
    TMatrixFixed<int,2,2> m;
    m.Clear();
    EXPECT_EQ(0, m(0, 1));
}

TEST_F(MatrixFixedTest, Fill)
{
    TMatrixFixed<int,2,2> m;
    m.Fill(1);
    EXPECT_EQ(1, m(1, 1));
}

TEST_F(MatrixFixedTest, Plus)
{
    TMatrixFixed<int,2,2> m1; m1.Fill(2);
    TMatrixFixed<int,2,2> m2; m2.Fill(3);
    TMatrixFixed<int,2,2> m3 = m1 + m2;
    EXPECT_EQ(5, m3(1, 1));
    m3 += m1;
    EXPECT_EQ(7, m3(0, 1));
    auto m4 = +m1;
    EXPECT_EQ(2, m4(1, 0));
}

TEST_F(MatrixFixedTest, Minus)
{
    TMatrixFixed<int,2,2> m1; m1.Fill(2);
    TMatrixFixed<int,2,2> m2; m2.Fill(3);
    TMatrixFixed<int,2,2> m3 = m1 - m2;
    EXPECT_EQ(-1, m3(1, 1));
    m3 -= m1;
    EXPECT_EQ(-3, m3(0, 1));
    auto m4 = -m1;
    EXPECT_EQ(-2, m4(1, 0));
}

TEST_F(MatrixFixedTest, Transpose)
{
    TMatrixFixed<int,2,2> m1;
    m1(0, 1) = 1;
    EXPECT_EQ(1, m1.Transpose()(1, 0));
}

TEST_F(MatrixFixedTest, Transposed)
{
    TMatrixFixed<int,2,2> m1;
    m1(0, 1) = 1;
    EXPECT_EQ(1, m1.Transposed()(1, 0));
    TMatrixFixed<int,2,3> m2;
    m2(0, 1) = 1;
    auto m3 = m2.Transposed();
    EXPECT_EQ(1, m3(1, 0));
    EXPECT_EQ(2, m3.Columns());
    EXPECT_EQ(3, m3.Rows());
}

