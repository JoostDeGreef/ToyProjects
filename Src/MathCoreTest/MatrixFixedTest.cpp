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

TEST_F(MatrixFixedTest, Eye)
{
    TMatrixFixed<int,3,2> m;
    m.Eye();
    EXPECT_EQ(1, m(1, 1));
    EXPECT_EQ(0, m(2, 1));
}

TEST_F(MatrixFixedTest, Diag)
{
    TMatrixFixed<int, 3, 2> m1({ 1,2,3,4,5,6 });
    auto res = m1.Diag();
    EXPECT_EQ(1,res(0));
    EXPECT_EQ(4,res(1));
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

TEST_F(MatrixFixedTest, Multiply)
{
    TMatrixFixed<int,2,2> m1; m1.Fill(2);
    TMatrixFixed<int,2,2> m2; m2.Fill(3);
    TMatrixFixed<int,2,2> m3 = m1 * m2;
    EXPECT_EQ(12, m3(1, 1));
    m1 *= m2;
    EXPECT_EQ(12, m1(0, 1));
    auto m4 = m1 * 3;
    EXPECT_EQ(36, m4(1, 0));
    m4 *= 2;
    EXPECT_EQ(72, m4(0, 0));
}

TEST_F(MatrixFixedTest, Equality)
{
    TMatrixFixed<int,2,2> m1({1,2,3,4});
    TMatrixFixed<int,2,2> m2({1,1,3,4});
    auto res = m1 == m2;
    EXPECT_TRUE(res(0, 0));
    EXPECT_FALSE(res(0, 1));
    EXPECT_TRUE(res(1, 0));
    EXPECT_TRUE(res(1, 1));
}

TEST_F(MatrixFixedTest, Inquality)
{
    TMatrixFixed<int,2,2> m1({1,2,3,4});
    TMatrixFixed<int,2,2> m2({1,1,3,4});
    auto res = m1 != m2;
    EXPECT_FALSE(res(0, 0));
    EXPECT_TRUE(res(0, 1));
    EXPECT_FALSE(res(1, 0));
    EXPECT_FALSE(res(1, 1));
}

TEST_F(MatrixFixedTest, Smaller)
{
    TMatrixFixed<int,2,2> m1({1,2,3,3});
    TMatrixFixed<int,2,2> m2({1,1,3,4});
    auto res = m1 < m2;
    EXPECT_FALSE(res(0, 0));
    EXPECT_FALSE(res(0, 1));
    EXPECT_FALSE(res(1, 0));
    EXPECT_TRUE(res(1, 1));
}

TEST_F(MatrixFixedTest, SmallerOrEqual)
{
    TMatrixFixed<int,2,2> m1({1,2,3,3});
    TMatrixFixed<int,2,2> m2({1,1,3,4});
    auto res = m1 <= m2;
    EXPECT_TRUE(res(0, 0));
    EXPECT_FALSE(res(0, 1));
    EXPECT_TRUE(res(1, 0));
    EXPECT_TRUE(res(1, 1));
}

TEST_F(MatrixFixedTest, Greater)
{
    TMatrixFixed<int,2,2> m1({1,2,3,3});
    TMatrixFixed<int,2,2> m2({1,1,3,4});
    auto res = m1 > m2;
    EXPECT_FALSE(res(0, 0));
    EXPECT_TRUE(res(0, 1));
    EXPECT_FALSE(res(1, 0));
    EXPECT_FALSE(res(1, 1));
}

TEST_F(MatrixFixedTest, GreaterOrEqual)
{
    TMatrixFixed<int,2,2> m1({1,2,3,3});
    TMatrixFixed<int,2,2> m2({1,1,3,4});
    auto res = m1 >= m2;
    EXPECT_TRUE(res(0, 0));
    EXPECT_TRUE(res(0, 1));
    EXPECT_TRUE(res(1, 0));
    EXPECT_FALSE(res(1, 1));
}



