#include "stdafx.h"
#include "shapes.h"
#include <GLInstanceVertex.h>

namespace nPhysics
{
	bt_cSphereShape::bt_cSphereShape(float radius)
		: iShape(SHAPE_TYPE_SPHERE)
		, mRadius(radius)
	{
        this->fallShape = new btSphereShape(radius);
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
        this->groundShape = new btStaticPlaneShape(btVector3(normal.x, normal.y, normal.z), planeConst);
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
    bt_cBoxShape::bt_cBoxShape(const glm::vec3& halfExtents)
        : iShape(SHAPE_TYPE_BOX)
    {
        btVector3 btExtents;
        btExtents.setX(halfExtents.x);
        btExtents.setY(halfExtents.y);
        btExtents.setZ(halfExtents.z);

        this->boxShape = new btBoxShape(btExtents);
    }
    bt_cBoxShape::~bt_cBoxShape()
    {
    }
    bt_cBoxShape & bt_cBoxShape::operator=(const bt_cBoxShape & other)
    {
        return *this;
    }
    bt_cConvexHullShape::bt_cConvexHullShape(const GLInstanceVertex* v, size_t numOfVertices)
        : iShape(SHAPE_TYPE_CONVEX_HULL)
    {
        this->convexHullShape = new btConvexHullShape((const btScalar*)(&(v->xyzw[0])),
                                                      numOfVertices,
                                                      sizeof(GLInstanceVertex));
    }
    bt_cConvexHullShape::~bt_cConvexHullShape()
    {
    }
    bt_cConvexHullShape & bt_cConvexHullShape::operator=(const btCollisionShape & other)
    {
        return *this;
    }
}