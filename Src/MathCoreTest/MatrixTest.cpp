#include "gtest/gtest.h"

#include "MathCore.h"

using namespace std;
using namespace testing;

class MatrixTest : public Test
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

TEST_F(MatrixTest, Constructor)
{
    TMatrix<int> m(2,2);
    EXPECT_EQ(0, m(0, 0));
}

TEST_F(MatrixTest, InitialisationList)
{
    TMatrix<int> m(2,2,{1,2,3,4});
    EXPECT_EQ(3, m(1, 0));
    EXPECT_EQ(4, m(1, 1));
}

TEST_F(MatrixTest, Copy)
{
    TMatrix<int> m1(2,2);
    m1.Fill(1);
    TMatrix<int> m2(m1);
    EXPECT_EQ(1, m2(0, 1));
}

TEST_F(MatrixTest, Clear)
{
    TMatrix<int> m(2,2);
    m.Clear();
    EXPECT_EQ(0, m(0, 1));
}

TEST_F(MatrixTest, Fill)
{
    TMatrix<int> m(2,2);
    m.Fill(1);
    EXPECT_EQ(1, m(1, 1));
}

TEST_F(MatrixTest, Eye)
{
    TMatrix<int> m(3, 2);
    m.Eye();
    EXPECT_EQ(1, m(1, 1));
    EXPECT_EQ(0, m(2, 1));
}

TEST_F(MatrixTest, Diag)
{
    TMatrix<int> m1(3,2,{ 1,2,3,4,5,6 });
    auto res = m1.Diag();
    EXPECT_EQ(1, res(0));
    EXPECT_EQ(4, res(1));
}

TEST_F(MatrixTest, Trace)
{
    TMatrix<int> m1(3,2,{ 2,3,4,5,6,7 });
    auto res = m1.Trace();
    EXPECT_EQ(10, res);
}

TEST_F(MatrixTest, Plus)
{
    TMatrix<int> m1(2, 2); m1.Fill(2);
    TMatrix<int> m2(2, 2); m2.Fill(3);
    TMatrix<int> m3 = m1 + m2;
    EXPECT_EQ(5, m3(1, 1));
    m3 += m1;
    EXPECT_EQ(7, m3(0, 1));
    auto m4 = +m1;
    EXPECT_EQ(2, m4(1, 0));
    auto m5 = m1 + 3;
    EXPECT_EQ(5, m5(1, 0));
    m5 += 2;
    EXPECT_EQ(7, m5(0, 0));
}

TEST_F(MatrixTest, Minus)
{
    TMatrix<int> m1(2, 2); m1.Fill(2);
    TMatrix<int> m2(2, 2); m2.Fill(3);
    TMatrix<int> m3 = m1 - m2;
    EXPECT_EQ(-1, m3(1, 1));
    m3 -= m1;
    EXPECT_EQ(-3, m3(0, 1));
    auto m4 = -m1;
    EXPECT_EQ(-2, m4(1, 0));
    auto m5 = m1 - 3;
    EXPECT_EQ(-1, m5(1, 0));
    m5 -= 1;
    EXPECT_EQ(-2, m5(0, 0));
}

TEST_F(MatrixTest, Transpose)
{
    TMatrix<int> m1(2, 2);
    m1(0, 1) = 1;
    EXPECT_EQ(1, m1.Transpose()(1, 0));
    TMatrix<int> m2(2, 3);
    m2(0, 1) = 1;
    m2.Transpose();
    EXPECT_EQ(1, m2(1, 0));
    EXPECT_EQ(2, m2.Columns());
    EXPECT_EQ(3, m2.Rows());
}

TEST_F(MatrixTest, Transposed)
{
    TMatrix<int> m1(2, 2);
    m1(0, 1) = 1;
    EXPECT_EQ(1, m1.Transposed()(1, 0));
    TMatrix<int> m2(2, 3);
    m2(0, 1) = 1;
    auto m3 = m2.Transposed();
    EXPECT_EQ(1, m3(1, 0));
    EXPECT_EQ(2, m3.Columns());
    EXPECT_EQ(3, m3.Rows());
}

TEST_F(MatrixTest, Multiply)
{
    TMatrix<int> m1(2, 2); m1.Fill(2);
    TMatrix<int> m2(2, 2); m2.Fill(3);
    TMatrix<int> m3 = m1 * m2;
    EXPECT_EQ(12, m3(1, 1));
    m1 *= m2;
    EXPECT_EQ(12, m1(0, 1));
    auto m4 = m1 * 3;
    EXPECT_EQ(36, m4(1, 0));
    m4 *= 2;
    EXPECT_EQ(72, m4(0, 0));
}

TEST_F(MatrixTest, Equality)
{
    TMatrix<int> m1(2, 2, {1,2,3,4});
    TMatrix<int> m2(2, 2, {1,1,3,4});
    auto res = m1 == m2;
    EXPECT_TRUE(res(0, 0));
    EXPECT_FALSE(res(0, 1));
    EXPECT_TRUE(res(1, 0));
    EXPECT_TRUE(res(1, 1));
}

TEST_F(MatrixTest, Inquality)
{
    TMatrix<int> m1(2, 2, {1,2,3,4});
    TMatrix<int> m2(2, 2, {1,1,3,4});
    auto res = m1 != m2;
    EXPECT_FALSE(res(0, 0));
    EXPECT_TRUE(res(0, 1));
    EXPECT_FALSE(res(1, 0));
    EXPECT_FALSE(res(1, 1));
}

TEST_F(MatrixTest, Smaller)
{
    TMatrix<int> m1(2, 2, {1,2,3,3});
    TMatrix<int> m2(2, 2, {1,1,3,4});
    auto res = m1 < m2;
    EXPECT_FALSE(res(0, 0));
    EXPECT_FALSE(res(0, 1));
    EXPECT_FALSE(res(1, 0));
    EXPECT_TRUE(res(1, 1));
}

TEST_F(MatrixTest, SmallerOrEqual)
{
    TMatrix<int> m1(2, 2, {1,2,3,3});
    TMatrix<int> m2(2, 2, {1,1,3,4});
    auto res = m1 <= m2;
    EXPECT_TRUE(res(0, 0));
    EXPECT_FALSE(res(0, 1));
    EXPECT_TRUE(res(1, 0));
    EXPECT_TRUE(res(1, 1));
}

TEST_F(MatrixTest, Greater)
{
    TMatrix<int> m1(2, 2, {1,2,3,3});
    TMatrix<int> m2(2, 2, {1,1,3,4});
    auto res = m1 > m2;
    EXPECT_FALSE(res(0, 0));
    EXPECT_TRUE(res(0, 1));
    EXPECT_FALSE(res(1, 0));
    EXPECT_FALSE(res(1, 1));
}

TEST_F(MatrixTest, GreaterOrEqual)
{
    TMatrix<int> m1(2, 2, {1,2,3,3});
    TMatrix<int> m2(2, 2, {1,1,3,4});
    auto res = m1 >= m2;
    EXPECT_TRUE(res(0, 0));
    EXPECT_TRUE(res(0, 1));
    EXPECT_TRUE(res(1, 0));
    EXPECT_FALSE(res(1, 1));
}

TEST_F(MatrixTest, BitInvert)
{
    TMatrix<unsigned int> m1(2, 2, {1,2,4,8});
    auto res = ~m1;
    EXPECT_TRUE(res(0)!=0);
    EXPECT_TRUE(res(1)!=0);
    EXPECT_TRUE(res(2)!=0);
    EXPECT_TRUE(res(3)!=0);
    EXPECT_EQ(0,res(0)&1);
    EXPECT_EQ(0,res(1)&2);
    EXPECT_EQ(0,res(2)&4);
    EXPECT_EQ(0,res(3)&8);
}

TEST_F(MatrixTest, BitOr)
{
    TMatrix<unsigned int> m1(2, 2, {1,2,4,8});
    auto res = m1 | 3;
    EXPECT_EQ(3,res(0));
    EXPECT_EQ(3,res(1));
    EXPECT_EQ(7,res(2));
    EXPECT_EQ(11,res(3));
    m1 |= 3;
    EXPECT_EQ(3,m1(0));
    EXPECT_EQ(3,m1(1));
    EXPECT_EQ(7,m1(2));
    EXPECT_EQ(11,m1(3));
}

TEST_F(MatrixTest, BitAnd)
{
    TMatrix<unsigned int> m1(2, 2, {1,2,4,8});
    auto res = m1 & 3;
    EXPECT_EQ(1,res(0));
    EXPECT_EQ(2,res(1));
    EXPECT_EQ(0,res(2));
    EXPECT_EQ(0,res(3));
    m1 &= 3;
    EXPECT_EQ(1,m1(0));
    EXPECT_EQ(2,m1(1));
    EXPECT_EQ(0,m1(2));
    EXPECT_EQ(0,m1(3));
}

TEST_F(MatrixTest, Cofactor)
{
    auto m0 = TMatrix<int>(3, 3, { 1,2,3, 0,4,5, 1,0,6 }).Cofactor();
    EXPECT_EQ(24, m0(0));
    EXPECT_EQ(5, m0(1));
    EXPECT_EQ(-4, m0(2));
    EXPECT_EQ(-12, m0(3));
    EXPECT_EQ(3, m0(4));
    EXPECT_EQ(2, m0(5));
    EXPECT_EQ(-2, m0(6));
    EXPECT_EQ(-5, m0(7));
    EXPECT_EQ(4, m0(8));
}

TEST_F(MatrixTest, Determinant)
{
    EXPECT_EQ(8,TMatrix<int>(1, 1, {8}).Determinant());    
    EXPECT_EQ(-2,TMatrix<int>(2, 2, {1,2,
                                     3,4}).Determinant());    
    EXPECT_EQ(18,TMatrix<int>(3, 3, {1,2,3,
                                     4,5,6,
                                     7,8,3}).Determinant());    
    EXPECT_EQ(112,TMatrix<int>(4, 4, {1,2,3,4,
                                      5,6,7,8,
                                      9,10,12,11,
                                      13,14,1,2}).Determinant());    
}

TEST_F(MatrixTest, Inverse)
{
    auto m0 = TMatrix<int>(3, 3, { 1,2,3,
                                   2,5,11,
                                   3,6,10 });
    auto m1 = m0.Inverse();
    auto m2 = m0 * m1;
    EXPECT_EQ(1, m2(0));
    EXPECT_EQ(0, m2(1));
    EXPECT_EQ(0, m2(2));
    EXPECT_EQ(0, m2(3));
    EXPECT_EQ(1, m2(4));
    EXPECT_EQ(0, m2(5));
    EXPECT_EQ(0, m2(6));
    EXPECT_EQ(0, m2(7));
    EXPECT_EQ(1, m2(8));
}

