#include "stdafx.h"
#include "shapes.h"
#include <GLInstanceVertex.h>

namespace nPhysics
{
	cSphereShape::cSphereShape(float radius)
		: iShape(SHAPE_TYPE_SPHERE)
		, mRadius(radius)
	{
        this->fallShape = new btSphereShape(radius);
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
        this->groundShape = new btStaticPlaneShape(btVector3(normal.x, normal.y, normal.z), planeConst);
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


    cBoxShape::cBoxShape(const glm::vec3& halfExtents)
        : iShape(SHAPE_TYPE_BOX)
    {
        btVector3 btExtents;
        btExtents.setX(halfExtents.x);
        btExtents.setY(halfExtents.y);
        btExtents.setZ(halfExtents.z);

        this->boxShape = new btBoxShape(btExtents);
    }

    cBoxShape::~cBoxShape()
    {
    }

    cBoxShape & cBoxShape::operator=(const cBoxShape & other)
    {
        return *this;
    }

    cConvexHullShape::cConvexHullShape(const GLInstanceVertex* v, size_t numOfVertices)
        : iShape(SHAPE_TYPE_CONVEX_HULL)
    {
        this->convexHullShape = new btConvexHullShape((const btScalar*)(&(v->xyzw[0])),
                                                      numOfVertices,
                                                      sizeof(GLInstanceVertex));
    }

    cConvexHullShape::~cConvexHullShape()
    {
    }

    cConvexHullShape & cConvexHullShape::operator=(const btCollisionShape & other)
    {
        return *this;
    }


    cCapsuleShape::cCapsuleShape(float radius, float height)
        : iShape(SHAPE_TYPE_CAPSULE)
    {
        this->capmsuleShape = new btCapsuleShape(radius, height);
    }

    cCapsuleShape::~cCapsuleShape()
    {
    }

    cCapsuleShape & cCapsuleShape::operator=(const btCollisionShape & other)
    {
        return *this;
    }
}