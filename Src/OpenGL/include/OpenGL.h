#pragma once

#include "MathCore.h"
#include "internal/RGBAColor.h"
#include "internal/RGBColor.h"

namespace OpenGL
{
	using Color = RGBAColorf;
	using ColorPtr = Color*;

	using TextureCoord = Vector2d;
	using TextureCoordPtr = TextureCoord*;

	using Vertex = Vector3d;
	using VertexPtr = Vertex*;

	using Normal = Vector3d;
	using NormalPtr = Normal*;

	void glColor(const RGBAColorf& color);
    void glColor(const RGBAColord& color);
    void glColor(const RGBColorf& color);
    void glColor(const RGBColord& color);
    inline void glColor(const ColorPtr& color) { glColor(*color); }

    void glVertex(const Vector3f& point);
    void glVertex(const Vector3d& point);
    inline void glVertex(const VertexPtr& point) { glVertex(*point); }
    void glVertex(const double& x, const double& y);
    void glVertex(const float& x, const float& y);

    void glNormal(const Vector3f& normal);
    void glNormal(const Vector3d& normal);
    inline void glNormal(const NormalPtr& normal) { glNormal(*normal); }

    void glTextureCoord(const Vector2f& coord);
    void glTextureCoord(const Vector2d& coord);
    inline void glTextureCoord(const TextureCoordPtr& coord) { glTextureCoord(*coord); }

    void glMultMatrix(const Quat& quat);

    void glLoadMatrix(const Quat& quat);
};

