#pragma once

#include <utility>

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
            
            bool LinesIntersect() const { return m_type[0] & Intersecting; } // colinear lines are special intersections, treated separately
            bool LinesParallel() const  { return m_type[0] & Parallel; }     // lines are parallel
            bool LinesColinear() const  { return m_type[0] & Colinear; }     // lines are colinear
            
            bool SegmentsSharePoint() const { return (m_type[0] & (Start|End)) && (m_type[1] & (Start|End)); } // end-end, start-start, end-start, start-end
            bool SegmentsIntersect() const { return (m_type[0] & (Start|On|End)) && (m_type[1] & (Start|On|End)); } // lines intersect within segments
            
            const double& s(const unsigned int index = 0) const { return m_s[index]; }
        private:
            point_type m_intersection;
            double m_s[2];
            uint16_t m_type[2];
        };

        Intersection CalculateIntersection(const TLine<POINT>& other, const value_type eps = 0.00001) const;
    private:
        point_type m_p1;
        point_type m_p2;
    };

    template<typename POINT>
    inline typename TLine<POINT>::Intersection TLine<POINT>::CalculateIntersection(const TLine<POINT>& other, const value_type eps) const
    {
        point_type slope0 = m_p2 - m_p1;
        point_type slope1 = other.m_p2 - other.m_p1;
        value_type den = slope0[0] * slope1[1] - slope0[1] * slope1[0];
        auto Interval = [](const value_type& s, const value_type& eps)
        {
            if (s + eps < 0)  { return Intersection::Before; }
            if (s <= eps)     { return Intersection::Start; }
            if (s + eps < 1)  { return Intersection::On; }
            if (s <= 1 + eps) { return Intersection::End; }
                                return Intersection::After;
        };
        if (den > eps || -den > eps)
        {
            point_type pivot = m_p1 - other.m_p1;
            value_type s = (pivot[1] * slope0[0] - pivot[0] * slope0[1]) / den;
            value_type t = (pivot[1] * slope1[0] - pivot[0] * slope1[1]) / den;
            point_type intersection = ((m_p1 + slope0 * t) + (other.m_p1 + slope1 * s)) * 0.5;
            const uint16_t types[2] = { static_cast<uint16_t>(Intersection::Intersecting|Interval(t, eps * slope0.Length())),
                                        static_cast<uint16_t>(Intersection::Intersecting|Interval(s, eps * slope1.Length())) };
            const value_type st[2] = { s,t };
            return Intersection(types, intersection, st);
        }
        else
        {
            point_type slope2 = m_p2 - other.m_p1;
            den = slope0[0] * slope2[1] - slope0[1] * slope2[0];
            if (den <= eps && -den <= eps)
            {
                // Check if slope0 and slope1 are opposite or not
                // calculate s and t
                value_type s = 0;
                value_type t = 0;
                const value_type st[2] = { s,t };
                const uint16_t types[2] = { static_cast<uint16_t>(Intersection::Colinear|Interval(st[0], eps * slope0.Length())),
                                            static_cast<uint16_t>(Intersection::Colinear|Interval(st[1], eps * slope1.Length())) };
                return Intersection(types, point_type(), st);
            }
            else
            {
                const value_type st[2] = { -1,-1 };
                const uint16_t types[2] = { Intersection::Parallel,Intersection::Parallel };
                return Intersection(types, point_type(), st);
            }
        }
    }

};

