#include "stdafx.h"
#include "shapes.h"

namespace nPhysics
{
	cSphereShape::cSphereShape(float radius)
		: iShape(SHAPE_TYPE_SPHERE)
		, mRadius(radius)
	{

	}
	cSphereShape::cSphereShape()
		: iShape(SHAPE_TYPE_SPHERE)
	{

	}

	cSphereShape::cSphereShape(const cSphereShape& other)
		: iShape(SHAPE_TYPE_SPHERE)
	{

	}
	cSphereShape& cSphereShape::operator=(const cSphereShape& other)
	{
		return *this;
	}
	cSphereShape::~cSphereShape()
	{

	}
	bool cSphereShape::GetSphereRadius(float& radiusOut)
	{
		radiusOut = mRadius;
		return true;
	}
	cPlaneShape::cPlaneShape(const glm::vec3& normal, float planeConst)
		: iShape(SHAPE_TYPE_PLANE)
		, mNormal(normal)
		, mPlaneConst(planeConst)
	{

	}
	cPlaneShape::cPlaneShape()
		: iShape(SHAPE_TYPE_PLANE)
	{

	}

	cPlaneShape::cPlaneShape(const cPlaneShape& other)
		: iShape(SHAPE_TYPE_PLANE)
	{

	}
	cPlaneShape& cPlaneShape::operator=(const cPlaneShape& other)
	{
		return *this;
	}
	cPlaneShape::~cPlaneShape()
	{

	}
	bool cPlaneShape::GetPlaneNormal(glm::vec3& normalOut)
	{
		normalOut = mNormal;
		return true;
	}
	bool cPlaneShape::GetPlaneConst(float& planeConstOut)
	{
		planeConstOut = mPlaneConst;
		return true;
	}
    cCubeShape::cCubeShape(float size)
    {
        this->cubeSize = size;
    }
    cCubeShape::~cCubeShape()
    {
    }
}