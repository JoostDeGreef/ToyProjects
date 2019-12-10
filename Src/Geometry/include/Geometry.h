#pragma once

#include "internal/2D/Point.h"
#include "internal/3D/Point.h"

template<typename T> struct is_point2d { static const bool value = false; };
template<> struct is_point2d<Geometry::Point2d> { static const bool value = true; };

template<typename T> struct is_point3d { static const bool value = false; };
template<> struct is_point2d<Geometry::Point3d> { static const bool value = true; };

#include "internal/2D/Line.h"
#include "internal/3D/Line.h"
