#pragma once

#include <utility>
#include <type_traits>

namespace Geometry
{
    // line segment
    // common code for 2D and 3D
    template<typename POINT>
    class TLine
    {
    public:
        typedef TLine<POINT> this_type;
        typedef POINT point_type;
        typedef typename point_type::Element value_type;

        TLine(const point_type& p1, const point_type& p2)
            : m_p1(p1)
            , m_p2(p2)
        {}
        TLine(const this_type& other)
            : m_p1(other.m_p1)
            , m_p2(other.m_p2)
        {}

        const point_type& GetPoint1() const { return m_p1; }
        const point_type& GetPoint2() const { return m_p2; }

        // parallel && colinear (= lines intersect, segments might intersect)
        //   ---- ----
        // parallel
        //   ----
        //     ----
        // !parallel (= lines intersect, segments might intersect)
        //  --- /
        //     /
        class Intersection
        {
        public:
            enum Type : uint16_t
            {
                // Lines not in the same plane
                Unconnected  = 0x0000,

                // The lines are intersecting, parallel, or parallel and colinear
                // only one of these flags should be set
                Intersecting = 0x1000,
                Parallel     = 0x2000,
                Colinear     = 0x4000,
                
                // In the case of a plain line intersection, these values
                // indicate the position of the intersection point on the
                // respective linesegment.
                //
                // In the case of colinear linesegments, these values 
                // indicate the positions of the first and second points
                // of the second segment on the first segment
                Before       = 0x0001,
                Start        = 0x0002,
                On           = 0x0004,
                End          = 0x0008,
                After        = 0x0010,
            };

            Intersection(const uint16_t type[2], const point_type& intersection, const double s[2])
                : m_intersection(intersection)
                , m_s{ s[0], s[1] }
                , m_type{ type[0], type[1] }
            {}

            const uint16_t GetPosition(const unsigned int index = 0) const { return m_type[index] & 0xFF; }
            const point_type& GetIntersection() const { return m_intersection; }
            const value_type& s(const unsigned int index = 0) const { return m_s[index]; }
            const auto& GetTypes() const { return m_type; }
            const auto& GetST() const { return m_s; }

            bool LinesIntersect() const { return m_type[0] & Intersecting; } // colinear lines are special intersections, treated separately
            bool LinesParallel() const  { return m_type[0] & Parallel; }     // lines are parallel
            bool LinesColinear() const  { return m_type[0] & Colinear; }     // lines are colinear
            
            bool SegmentsSharePoint() const { return (m_type[0] & (Start|End)) && (m_type[1] & (Start|End)); } // end-end, start-start, end-start, start-end
            bool SegmentsIntersect() const { return (m_type[0] & (Start|On|End)) && (m_type[1] & (Start|On|End)); } // lines intersect within segments
            
        private:
            point_type m_intersection;
            value_type m_s[2];
            uint16_t m_type[2];
        };
        
        Intersection CalculateIntersection(const this_type& other, const value_type eps = 0.00001) const;
    private:
        point_type m_p1;
        point_type m_p2;
    };

    namespace details
    {
        template<typename POINT, typename std::enable_if<is_point2<POINT>::value, int>::type = 0>
        inline typename TLine<POINT>::Intersection CalculateIntersection(const POINT &l1p1, const POINT &l1p2, const POINT &l2p1, const POINT &l2p2, const typename POINT::Element eps)
        {
            auto slope0 = l1p2 - l1p1;
            auto slope1 = l2p2 - l2p1;
            auto den = slope0[0] * slope1[1] - slope0[1] * slope1[0];
            auto Interval = [](const auto& s, const auto& eps)
            {
                if (s + eps < 0)  { return TLine<POINT>::Intersection::Before; }
                if (s <= eps)     { return TLine<POINT>::Intersection::Start; }
                if (s + eps < 1)  { return TLine<POINT>::Intersection::On; }
                if (s <= 1 + eps) { return TLine<POINT>::Intersection::End; }
                                    return TLine<POINT>::Intersection::After;
            };
            if (den > eps || -den > eps)
            {
                auto pivot = l1p1 - l2p1;
                auto s = (pivot[1] * slope0[0] - pivot[0] * slope0[1]) / den;
                auto t = (pivot[1] * slope1[0] - pivot[0] * slope1[1]) / den;
                auto intersection = ((l1p1 + slope0 * t) + (l2p1 + slope1 * s)) * 0.5;
                const uint16_t types[2] = { static_cast<uint16_t>(TLine<POINT>::Intersection::Intersecting|Interval(t, eps * slope0.Length())),
                                            static_cast<uint16_t>(TLine<POINT>::Intersection::Intersecting|Interval(s, eps * slope1.Length())) };
                const decltype(s) st[2] = { s,t };
                return typename TLine<POINT>::Intersection(types, intersection, st);
            }
            else
            {
                auto slope2 = l1p2 - l2p1;
                den = slope0[0] * slope2[1] - slope0[1] * slope2[0];
                if (den <= eps && -den <= eps)
                {
                    // TODO:
                    // - Check if slope0 and slope1 are opposite or not
                    // - Calculate s and t
                    auto s = 0.0;
                    auto t = 0.0;
                    const decltype(s) st[2] = { s,t };
                    const uint16_t types[2] = { static_cast<uint16_t>(TLine<POINT>::Intersection::Colinear|Interval(st[0], eps * slope0.Length())),
                                                static_cast<uint16_t>(TLine<POINT>::Intersection::Colinear|Interval(st[1], eps * slope1.Length())) };
                    return typename TLine<POINT>::Intersection(types, POINT(), st);
                }
                else
                {
                    const typename POINT::Element st[2] = { -1,-1 };
                    const uint16_t types[2] = { TLine<POINT>::Intersection::Parallel,TLine<POINT>::Intersection::Parallel };
                    return typename TLine<POINT>::Intersection(types, POINT(), st);
                }
            }
        }
        
        template<typename POINT, typename std::enable_if<is_point3<POINT>::value, int>::type = 0>
        inline typename TLine<POINT>::Intersection CalculateIntersection(const POINT &l1p1, const POINT &l1p2, const POINT &l2p1, const POINT &l2p2, const typename POINT::Element eps)
        {
            // Steps:
            // - see if the two lines share a plane.
            // - map the points to 2d points on the plane.
            // - calculate intersection
            // - map calculated 2d points back to 3d space
            auto v0 = l1p2 - l1p1; // line 1
            auto v1 = l2p1 - l1p1; 
            auto v2 = l2p2 - l1p1;
            auto n = CrossProduct(v0, v1);
            auto m = CrossProduct(v0, v2);
            auto nl = n.Length();
            auto ml = m.Length();
            POINT::Element d;
            if (nl > ml)
            {
                n /= nl;
                d = v2.InnerProduct(n);
            }
            else
            {
                n = m / ml;
                d = v1.InnerProduct(n);
            }
            if (d<-eps || d>eps)
            {
                const typename POINT::Element st[2] = { -1,-1 };
                const uint16_t types[2] = { TLine<POINT>::Intersection::Unconnected,TLine<POINT>::Intersection::Unconnected };
                return typename TLine<POINT>::Intersection(types, POINT(), st);
            }
            else
            {
                auto e0 = v0.Normalized();
                auto e1 = CrossProduct(e0, n).Normalized();
                auto Proj = [&e0,&e1](POINT & p)
                {
                    auto t0 = e0.InnerProduct(p);
                    auto t1 = e1.InnerProduct(p);
                    return Point2<typename POINT::Element>(t0, t1);
                };
                Point2<typename POINT::Element> a(0,0), b=Proj(v0), c=Proj(v1), d=Proj(v2);
                auto res2d = CalculateIntersection(a, b, c, d, eps);
                if (res2d.LinesIntersect())
                {
                    const auto& st = res2d.GetST();
                    const auto& types = res2d.GetTypes();
                    auto v3 = l2p2 - l2p1; // line 2
                    POINT p = (l1p1 + st[1] * v0 + l2p1 + st[0] * v3)/2;
                    return typename TLine<POINT>::Intersection(types, p, st);
                }
                else if (res2d.LinesParallel())
                {
                    const auto& st = res2d.GetST();
                    const auto& types = res2d.GetTypes();
                    return typename TLine<POINT>::Intersection(types, POINT(-1,-1,-1), st);
                }
                else
                {
                    assert(res2d.LinesColinear());
                    // todo
                    const auto& st = res2d.GetST();
                    const auto& types = res2d.GetTypes();
                    auto v3 = l2p2 - l2p1; // line 2
                    POINT p = (l1p1 + st[1] * v0 + l2p1 + st[0] * v3) / 2;
                    return typename TLine<POINT>::Intersection(types, p, st);
                }
            }
        }
    }

    template<typename POINT>
    inline typename TLine<POINT>::Intersection TLine<POINT>::CalculateIntersection(const TLine<POINT>& other, const value_type eps) const
    {
        return details::CalculateIntersection(m_p1,m_p2,other.m_p1,other.m_p2,eps);
    }
};

