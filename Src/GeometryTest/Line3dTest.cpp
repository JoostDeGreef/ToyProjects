#include "gtest/gtest.h"
#include "Geometry.h"

using namespace std;
using namespace testing;
using namespace Geometry;

class Line3dTest : public Test
{
protected:
    virtual void SetUp()
    {
    }

    virtual void TearDown()
    {
    }

    double distance(const Line3d::point_type & p0, const Line3d::point_type & p1)
    {
        return (p0 - p1).Length();
    }

    void Test(Line3d l0, Line3d l1, Line3d::point_type p, Line3d::Intersection::Type pos0, Line3d::Intersection::Type pos1, bool linesParallel, bool linesIntersect, bool segmentsSharePoint, bool segmentsIntersect)
    {
        auto intersection = l0.CalculateIntersection(l1);
        EXPECT_EQ(linesParallel, intersection.LinesParallel());
        EXPECT_EQ(linesIntersect, intersection.LinesIntersect());
        EXPECT_EQ(segmentsSharePoint, intersection.SegmentsSharePoint());
        EXPECT_EQ(segmentsIntersect, intersection.SegmentsIntersect());
        EXPECT_EQ(pos0, intersection.GetPosition(0));
        EXPECT_EQ(pos1, intersection.GetPosition(1));
        if (!linesParallel) EXPECT_EQ(0, distance(p, intersection.GetIntersection())) << p << intersection.GetIntersection();
    }
};


TEST_F(Line3dTest, Parallel)
{
    Test({ { 0,0,0 },{ 1,0,0 } }, { { 0,1,0 },{ 1,1,0 } }, { 0,0,0 }, Line3d::Intersection::Unconnected, Line3d::Intersection::Unconnected, true, false, false, false);
}

TEST_F(Line3dTest, BeforeBefore)
{
    Test({ { 0,0,1 },{ 1,0,1 } }, { { 0,1,1 },{ 1,2,1 } }, { -1,0,1 }, Line3d::Intersection::Before, Line3d::Intersection::Before, false, true, false, false);
}

TEST_F(Line3dTest, BeforeStart)
{
    Test({ { 0,0,2 },{ 1,0,2 } }, { { -1,0,2 },{ 1,1,2 } }, { -1,0,2 }, Line3d::Intersection::Before, Line3d::Intersection::Start, false, true, false, false);
}

TEST_F(Line3dTest, BeforeOn)
{
    Test({ { 0,0,3 },{ 1,0,3 } }, { { 0,1,3 },{-1,-1,3 } }, { -0.5,0,3 }, Line3d::Intersection::Before, Line3d::Intersection::On, false, true, false, false);
}

TEST_F(Line3dTest, BeforeEnd)
{
    Test({ { 0,0,4 },{ 1,0,4 } }, { {-1,1,4 },{-1,0,4 } }, { -1,0,4 }, Line3d::Intersection::Before, Line3d::Intersection::End, false, true, false, false);
}

TEST_F(Line3dTest, BeforeAfter)
{
    Test({ { 0,0,5 },{ 1,0,5 } }, { {-1,2,5 },{-1,1,5 } }, { -1,0,5 }, Line3d::Intersection::Before, Line3d::Intersection::After, false, true, false, false);
}

TEST_F(Line3dTest, StartBefore)
{
    Test({ { 0,0,6 },{ 1,0,6 } }, { { 0,1,6 },{ 0,2,6 } }, { 0,0,6 }, Line3d::Intersection::Start, Line3d::Intersection::Before, false, true, false, false);
}

TEST_F(Line3dTest, StartStart)
{
    Test({ { 0,0,7 },{ 1,0,7 } }, { { 0,0,7 },{ 1,1,7 } }, { 0,0,7 }, Line3d::Intersection::Start, Line3d::Intersection::Start, false, true, true, true);
}

TEST_F(Line3dTest, StartOn)
{
    Test({ { 0,0,8 },{ 1,0,8 } }, { {-1,-1,8 },{ 1,1,8 } }, { 0,0,8 }, Line3d::Intersection::Start, Line3d::Intersection::On, false, true, false, true);
}

TEST_F(Line3dTest, StartEnd)
{
    Test({ { 0,1,9 },{ 1,1,9 } }, { {-1,-1,9 },{ 0,1,9 } }, { 0,1,9 }, Line3d::Intersection::Start, Line3d::Intersection::End, false, true, true, true);
}

TEST_F(Line3dTest, StartAfter)
{
    Test({ { 0,0,6 },{ 1,0,6 } }, { { 0,2,6 },{ 0,1,6 } }, { 0,0,6 }, Line3d::Intersection::Start, Line3d::Intersection::After, false, true, false, false);
}



TEST_F(Line3dTest, OnBefore)
{
    Test({ {-1,0,4 },{ 1,0,4 } }, { { 0,1,4 },{ 0,2,4 } }, { 0,0,4 }, Line3d::Intersection::On, Line3d::Intersection::Before, false, true, false, false);
}

TEST_F(Line3dTest, OnStart)
{
    Test({ {-1,0,3 },{ 1,0,3 } }, { { 0,0,3 },{ 1,1,3 } }, { 0,0,3 }, Line3d::Intersection::On, Line3d::Intersection::Start, false, true, false, true);
}

TEST_F(Line3dTest, OnOn)
{
    Test({ { -1,0,2 },{ 1,0,2 } }, { { 0,1,2 },{ 0,-1,2 } }, { 0,0,2 }, Line3d::Intersection::On, Line3d::Intersection::On, false, true, false, true);
}

TEST_F(Line3dTest, OnEnd)
{
    Test({ {-1,0,1 },{ 1,0,1 } }, { { 0,1,1 },{ 0,0,1 } }, { 0,0,1 }, Line3d::Intersection::On, Line3d::Intersection::End, false, true, false, true);
}

TEST_F(Line3dTest, OnAfter)
{
    Test({ {-1,0,0 },{ 1,0,0 } }, { { 0,2,0 },{ 0,1,0 } }, { 0,0,0 }, Line3d::Intersection::On, Line3d::Intersection::After, false, true, false, false);
}



TEST_F(Line3dTest, EndBefore)
{
    Test({ {-1,0,4 },{ 1,0,4 } }, { { 0,1,4 },{-1,2,4 } }, { 1,0,4 }, Line3d::Intersection::End, Line3d::Intersection::Before, false, true, false, false);
}

TEST_F(Line3dTest, EndStart)
{
    Test({ { 0,0,3 },{ 1,0,3 } }, { { 1,0,3 },{ 1,1,3 } }, { 1,0,3 }, Line3d::Intersection::End, Line3d::Intersection::Start, false, true, true, true);
}

TEST_F(Line3dTest, EndOn)
{
    Test({ { 0,0,2 },{ 1,0,2 } }, { { 1,1,2 },{ 1,-1,2 } }, { 1,0,2 }, Line3d::Intersection::End, Line3d::Intersection::On, false, true, false, true);
}

TEST_F(Line3dTest, EndEnd)
{
    Test({ { 0,0,1 },{ 1,0,1 } }, { { 0,1,1 },{ 1,0,1 } }, { 1,0,1 }, Line3d::Intersection::End, Line3d::Intersection::End, false, true, true, true);
}

TEST_F(Line3dTest, EndAfter)
{
    Test({ {-1,0,0 },{ 1,0,0 } }, { {-1,2,0 },{ 0,1,0 } }, { 1,0,0 }, Line3d::Intersection::End, Line3d::Intersection::After, false, true, false, false);
}



TEST_F(Line3dTest, AfterBefore)
{
    Test({ { 0,2,4 },{ 0,1,4 } }, { { 1,0,4 },{ 3,0,4 } }, { 0,0,4 }, Line3d::Intersection::After, Line3d::Intersection::Before, false, true, false, false);
}

TEST_F(Line3dTest, AfterStart)
{
    Test({ { 0,3,3 },{ 0,1,3 } }, { { 0,0,3 },{ 1,0,3 } }, { 0,0,3 }, Line3d::Intersection::After, Line3d::Intersection::Start, false, true, false, false);
}

TEST_F(Line3dTest, AfterOn)
{
    Test({ { 0,3,2 },{ 0,1,2 } }, { { -1,0,2 },{ 1,0,2 } }, { 0,0,2 }, Line3d::Intersection::After, Line3d::Intersection::On, false, true, false, false);
}

TEST_F(Line3dTest, AfterEnd)
{
    Test({ { 0,3,1 },{ 0,1,1 } }, { { 1,0,1 },{ 0,0,1 } }, { 0,0,1 }, Line3d::Intersection::After, Line3d::Intersection::End, false, true, false, false);
}

TEST_F(Line3dTest, AfterAfter)
{
    Test({ { 0,3,0 },{ 0,1,0 } }, { { 2,0,0 },{ 1,0,0 } }, { 0,0,0 }, Line3d::Intersection::After, Line3d::Intersection::After, false, true, false, false);
}


/*
TEST_F(Line2dTest, Colinear)
{
    Test({ { 0,0 },{ 0,1 } }, { { 0,2 },{ 0,3 } }, { 0,0 }, Line2d::Intersection::After, Line2d::Intersection::After, true, false, false, false);
}

*/