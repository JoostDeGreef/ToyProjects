#include "gtest/gtest.h"

#include "MathCore.h"

using namespace std;
using namespace testing;

class MatrixStaticTest : public Test
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

TEST_F(MatrixStaticTest, Constructor)
{
    TMatrix<int,2,2> m;
    EXPECT_EQ(0, m(0, 0));
}

TEST_F(MatrixStaticTest, Copy)
{
    TMatrix<int,2,2> m1;
    m1.Fill(1);
    TMatrix<int,2,2> m2(m1);
    EXPECT_EQ(1, m2(0, 1));
}

TEST_F(MatrixStaticTest, Clear)
{
    TMatrix<int,2,2> m;
    m.Clear();
    EXPECT_EQ(0, m(0, 1));
}

TEST_F(MatrixStaticTest, Fill)
{
    TMatrix<int,2,2> m;
    m.Fill(1);
    EXPECT_EQ(1, m(1, 1));
}

TEST_F(MatrixStaticTest, Eye)
{
    TMatrix<int,3,2> m;
    m.Eye();
    EXPECT_EQ(1, m(1, 1));
    EXPECT_EQ(0, m(2, 1));
}

TEST_F(MatrixStaticTest, Diag)
{
    TMatrix<int, 3, 2> m1({ 1,2,3,4,5,6 });
    auto res = m1.Diag();
    EXPECT_EQ(1,res(0));
    EXPECT_EQ(4,res(1));
}

TEST_F(MatrixStaticTest, Plus)
{
    TMatrix<int,2,2> m1; m1.Fill(2);
    TMatrix<int,2,2> m2; m2.Fill(3);
    TMatrix<int,2,2> m3 = m1 + m2;
    EXPECT_EQ(5, m3(1, 1));
    m3 += m1;
    EXPECT_EQ(7, m3(0, 1));
    auto m4 = +m1;
    EXPECT_EQ(2, m4(1, 0));
}

TEST_F(MatrixStaticTest, Minus)
{
    TMatrix<int,2,2> m1; m1.Fill(2);
    TMatrix<int,2,2> m2; m2.Fill(3);
    TMatrix<int,2,2> m3 = m1 - m2;
    EXPECT_EQ(-1, m3(1, 1));
    m3 -= m1;
    EXPECT_EQ(-3, m3(0, 1));
    auto m4 = -m1;
    EXPECT_EQ(-2, m4(1, 0));
}

TEST_F(MatrixStaticTest, Transpose)
{
    TMatrix<int,2,2> m1;
    m1(0, 1) = 1;
    EXPECT_EQ(1, m1.Transpose()(1, 0));
}

TEST_F(MatrixStaticTest, Transposed)
{
    TMatrix<int,2,2> m1;
    m1(0, 1) = 1;
    EXPECT_EQ(1, m1.Transposed()(1, 0));
    TMatrix<int,2,3> m2;
    m2(0, 1) = 1;
    auto m3 = m2.Transposed();
    EXPECT_EQ(1, m3(1, 0));
    EXPECT_EQ(2, m3.Columns());
    EXPECT_EQ(3, m3.Rows());
}

TEST_F(MatrixStaticTest, Multiply)
{
    TMatrix<int,2,2> m1; m1.Fill(2);
    TMatrix<int,2,2> m2; m2.Fill(3);
    TMatrix<int,2,2> m3 = m1 * m2;
    EXPECT_EQ(12, m3(1, 1));
    m1 *= m2;
    EXPECT_EQ(12, m1(0, 1));
    auto m4 = m1 * 3;
    EXPECT_EQ(36, m4(1, 0));
    m4 *= 2;
    EXPECT_EQ(72, m4(0, 0));
}

TEST_F(MatrixStaticTest, Equality)
{
    TMatrix<int,2,2> m1({1,2,3,4});
    TMatrix<int,2,2> m2({1,1,3,4});
    auto res = m1 == m2;
    EXPECT_TRUE(res(0, 0));
    EXPECT_FALSE(res(0, 1));
    EXPECT_TRUE(res(1, 0));
    EXPECT_TRUE(res(1, 1));
}

TEST_F(MatrixStaticTest, Inquality)
{
    TMatrix<int,2,2> m1({1,2,3,4});
    TMatrix<int,2,2> m2({1,1,3,4});
    auto res = m1 != m2;
    EXPECT_FALSE(res(0, 0));
    EXPECT_TRUE(res(0, 1));
    EXPECT_FALSE(res(1, 0));
    EXPECT_FALSE(res(1, 1));
}

TEST_F(MatrixStaticTest, Smaller)
{
    TMatrix<int,2,2> m1({1,2,3,3});
    TMatrix<int,2,2> m2({1,1,3,4});
    auto res = m1 < m2;
    EXPECT_FALSE(res(0, 0));
    EXPECT_FALSE(res(0, 1));
    EXPECT_FALSE(res(1, 0));
    EXPECT_TRUE(res(1, 1));
}

TEST_F(MatrixStaticTest, SmallerOrEqual)
{
    TMatrix<int,2,2> m1({1,2,3,3});
    TMatrix<int,2,2> m2({1,1,3,4});
    auto res = m1 <= m2;
    EXPECT_TRUE(res(0, 0));
    EXPECT_FALSE(res(0, 1));
    EXPECT_TRUE(res(1, 0));
    EXPECT_TRUE(res(1, 1));
}

TEST_F(MatrixStaticTest, Greater)
{
    TMatrix<int,2,2> m1({1,2,3,3});
    TMatrix<int,2,2> m2({1,1,3,4});
    auto res = m1 > m2;
    EXPECT_FALSE(res(0, 0));
    EXPECT_TRUE(res(0, 1));
    EXPECT_FALSE(res(1, 0));
    EXPECT_FALSE(res(1, 1));
}

TEST_F(MatrixStaticTest, GreaterOrEqual)
{
    TMatrix<int,2,2> m1({1,2,3,3});
    TMatrix<int,2,2> m2({1,1,3,4});
    auto res = m1 >= m2;
    EXPECT_TRUE(res(0, 0));
    EXPECT_TRUE(res(0, 1));
    EXPECT_TRUE(res(1, 0));
    EXPECT_FALSE(res(1, 1));
}



