#include "stdafx.h"
#include "bt_shapes.h"

namespace nPhysics
{
	bt_cSphereShape::bt_cSphereShape(float radius)
		: iShape(SHAPE_TYPE_SPHERE)
		, mRadius(radius)
	{

	}
	bt_cSphereShape::bt_cSphereShape()
		: iShape(SHAPE_TYPE_SPHERE)
	{

	}

	bt_cSphereShape::bt_cSphereShape(const bt_cSphereShape& other)
		: iShape(SHAPE_TYPE_SPHERE)
	{

	}
	bt_cSphereShape& bt_cSphereShape::operator=(const bt_cSphereShape& other)
	{
		return *this;
	}
	bt_cSphereShape::~bt_cSphereShape()
	{

	}
	bool bt_cSphereShape::GetSphereRadius(float& radiusOut)
	{
		radiusOut = mRadius;
		return true;
	}
	bt_cPlaneShape::bt_cPlaneShape(const glm::vec3& normal, float planeConst)
		: iShape(SHAPE_TYPE_PLANE)
		, mNormal(normal)
		, mPlaneConst(planeConst)
	{

	}
	bt_cPlaneShape::bt_cPlaneShape()
		: iShape(SHAPE_TYPE_PLANE)
	{

	}

	bt_cPlaneShape::bt_cPlaneShape(const bt_cPlaneShape& other)
		: iShape(SHAPE_TYPE_PLANE)
	{

	}
	bt_cPlaneShape& bt_cPlaneShape::operator=(const bt_cPlaneShape& other)
	{
		return *this;
	}
	bt_cPlaneShape::~bt_cPlaneShape()
	{

	}
	bool bt_cPlaneShape::GetPlaneNormal(glm::vec3& normalOut)
	{
		normalOut = mNormal;
		return true;
	}
	bool bt_cPlaneShape::GetPlaneConst(float& planeConstOut)
	{
		planeConstOut = mPlaneConst;
		return true;
	}
    bt_cCubeShape::bt_cCubeShape(float size)
    {
        this->cubeSize = size;
    }
    bt_cCubeShape::~bt_cCubeShape()
    {
    }
}