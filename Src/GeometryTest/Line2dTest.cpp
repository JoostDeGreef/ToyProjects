#include "gtest/gtest.h"
#include "Geometry.h"

using namespace std;
using namespace testing;
using namespace Geometry;

class Line2dTest : public Test
{
protected:
    virtual void SetUp()
    {
    }

    virtual void TearDown()
    {
    }

    double distance(const Line2d::point_type & p0, const Line2d::point_type & p1)
    {
        return (p0 - p1).Length();
    }

    void Test(Line2d l0, Line2d l1, Line2d::point_type p, Line2d::Intersection::Type pos0, Line2d::Intersection::Type pos1, bool linesParallel, bool linesIntersect, bool segmentsSharePoint, bool segmentsIntersect)
    {
        auto intersection = l0.CalculateIntersection(l1);
        EXPECT_EQ(linesParallel, intersection.LinesParallel());
        EXPECT_EQ(linesIntersect, intersection.LinesIntersect());
        EXPECT_EQ(segmentsSharePoint, intersection.SegmentsSharePoint());
        EXPECT_EQ(segmentsIntersect, intersection.SegmentsIntersect());
        EXPECT_EQ(pos0, intersection.GetPosition(0));
        EXPECT_EQ(pos1, intersection.GetPosition(1));
        if (!linesParallel) EXPECT_EQ(0, distance(p, intersection.GetIntersection()));
    }
};


TEST_F(Line2dTest, Parallel)
{
    Test({ { 0,0 },{ 1,0 } }, { { 0,1 },{ 1,1 } }, { 0,0 }, Line2d::Intersection::Unconnected, Line2d::Intersection::Unconnected, true, false, false, false);
}

TEST_F(Line2dTest, BeforeBefore)
{
    Test({ { 0,0 },{ 1,0 } }, { { 0,1 },{ 1,2 } }, { -1,0 }, Line2d::Intersection::Before, Line2d::Intersection::Before, false, true, false, false);
}

TEST_F(Line2dTest, BeforeStart)
{
    Test({ { 0,0 },{ 1,0 } }, { { -1,0 },{ 1,1 } }, { -1,0 }, Line2d::Intersection::Before, Line2d::Intersection::Start, false, true, false, false);
}

TEST_F(Line2dTest, BeforeOn)
{
    Test({ { 0,0 },{ 1,0 } }, { { 0,1 },{-1,-1 } }, { -0.5,0 }, Line2d::Intersection::Before, Line2d::Intersection::On, false, true, false, false);
}

TEST_F(Line2dTest, BeforeEnd)
{
    Test({ { 0,0 },{ 1,0 } }, { {-1,1 },{-1,0 } }, { -1,0 }, Line2d::Intersection::Before, Line2d::Intersection::End, false, true, false, false);
}

TEST_F(Line2dTest, BeforeAfter)
{
    Test({ { 0,0 },{ 1,0 } }, { {-1,2 },{-1,1 } }, { -1,0 }, Line2d::Intersection::Before, Line2d::Intersection::After, false, true, false, false);
}

TEST_F(Line2dTest, StartBefore)
{
    Test({ { 0,0 },{ 1,0 } }, { { 0,1 },{ 0,2 } }, { 0,0 }, Line2d::Intersection::Start, Line2d::Intersection::Before, false, true, false, false);
}

TEST_F(Line2dTest, StartStart)
{
    Test({ { 0,0 },{ 1,0 } }, { { 0,0 },{ 1,1 } }, { 0,0 }, Line2d::Intersection::Start, Line2d::Intersection::Start, false, true, true, true);
}

TEST_F(Line2dTest, StartOn)
{
    Test({ { 0,0 },{ 1,0 } }, { {-1,-1 },{ 1,1 } }, { 0,0 }, Line2d::Intersection::Start, Line2d::Intersection::On, false, true, false, true);
}

TEST_F(Line2dTest, StartEnd)
{
    Test({ { 0,1 },{ 1,1 } }, { {-1,-1 },{ 0,1 } }, { 0,1 }, Line2d::Intersection::Start, Line2d::Intersection::End, false, true, true, true);
}

TEST_F(Line2dTest, StartAfter)
{
    Test({ { 0,0 },{ 1,0 } }, { { 0,2 },{ 0,1 } }, { 0,0 }, Line2d::Intersection::Start, Line2d::Intersection::After, false, true, false, false);
}



TEST_F(Line2dTest, OnBefore)
{
    Test({ {-1,0 },{ 1,0 } }, { { 0,1 },{ 0,2 } }, { 0,0 }, Line2d::Intersection::On, Line2d::Intersection::Before, false, true, false, false);
}

TEST_F(Line2dTest, OnStart)
{
    Test({ {-1,0 },{ 1,0 } }, { { 0,0 },{ 1,1 } }, { 0,0 }, Line2d::Intersection::On, Line2d::Intersection::Start, false, true, false, true);
}

TEST_F(Line2dTest, OnOn)
{
    Test({ { -1,0 },{ 1,0 } }, { { 0,1 },{ 0,-1 } }, { 0,0 }, Line2d::Intersection::On, Line2d::Intersection::On, false, true, false, true);
}

TEST_F(Line2dTest, OnEnd)
{
    Test({ {-1,0 },{ 1,0 } }, { { 0,1 },{ 0,0 } }, { 0,0 }, Line2d::Intersection::On, Line2d::Intersection::End, false, true, false, true);
}

TEST_F(Line2dTest, OnAfter)
{
    Test({ {-1,0 },{ 1,0 } }, { { 0,2 },{ 0,1 } }, { 0,0 }, Line2d::Intersection::On, Line2d::Intersection::After, false, true, false, false);
}



TEST_F(Line2dTest, EndBefore)
{
    Test({ {-1,0 },{ 1,0 } }, { { 0,1 },{-1,2 } }, { 1,0 }, Line2d::Intersection::End, Line2d::Intersection::Before, false, true, false, false);
}

TEST_F(Line2dTest, EndStart)
{
    Test({ { 0,0 },{ 1,0 } }, { { 1,0 },{ 1,1 } }, { 1,0 }, Line2d::Intersection::End, Line2d::Intersection::Start, false, true, true, true);
}

TEST_F(Line2dTest, EndOn)
{
    Test({ { 0,0 },{ 1,0 } }, { { 1,1 },{ 1,-1 } }, { 1,0 }, Line2d::Intersection::End, Line2d::Intersection::On, false, true, false, true);
}

TEST_F(Line2dTest, EndEnd)
{
    Test({ { 0,0 },{ 1,0 } }, { { 0,1 },{ 1,0 } }, { 1,0 }, Line2d::Intersection::End, Line2d::Intersection::End, false, true, true, true);
}

TEST_F(Line2dTest, EndAfter)
{
    Test({ {-1,0 },{ 1,0 } }, { {-1,2 },{ 0,1 } }, { 1,0 }, Line2d::Intersection::End, Line2d::Intersection::After, false, true, false, false);
}



TEST_F(Line2dTest, AfterBefore)
{
    Test({ { 0,2 },{ 0,1 } }, { { 1,0 },{ 3,0 } }, { 0,0 }, Line2d::Intersection::After, Line2d::Intersection::Before, false, true, false, false);
}

TEST_F(Line2dTest, AfterStart)
{
    Test({ { 0,3 },{ 0,1 } }, { { 0,0 },{ 1,0 } }, { 0,0 }, Line2d::Intersection::After, Line2d::Intersection::Start, false, true, false, false);
}

TEST_F(Line2dTest, AfterOn)
{
    Test({ { 0,3 },{ 0,1 } }, { { -1,0 },{ 1,0 } }, { 0,0 }, Line2d::Intersection::After, Line2d::Intersection::On, false, true, false, false);
}

TEST_F(Line2dTest, AfterEnd)
{
    Test({ { 0,3 },{ 0,1 } }, { { 1,0 },{ 0,0 } }, { 0,0 }, Line2d::Intersection::After, Line2d::Intersection::End, false, true, false, false);
}

TEST_F(Line2dTest, AfterAfter)
{
    Test({ { 0,3 },{ 0,1 } }, { { 2,0 },{ 1,0 } }, { 0,0 }, Line2d::Intersection::After, Line2d::Intersection::After, false, true, false, false);
}


/*
TEST_F(Line2dTest, Colinear)
{
    Test({ { 0,0 },{ 0,1 } }, { { 0,2 },{ 0,3 } }, { 0,0 }, Line2d::Intersection::After, Line2d::Intersection::After, true, false, false, false);
}

*/