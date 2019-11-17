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
