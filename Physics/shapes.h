#pragma once
#include <glm\vec3.hpp>
#include <iShape.h>
#include <btBulletDynamicsCommon.h>

namespace nPhysics
{
    struct GLInstanceVertex;

	class bt_cSphereShape : public iShape
	{
	public:
		bt_cSphereShape(float radius);

		virtual ~bt_cSphereShape();

		virtual bool GetSphereRadius(float& radiusOut);

        inline float getRadius() { return this->mRadius; }

        inline btCollisionShape* getBulletShape() { return this->fallShape; }

	private:
		bt_cSphereShape();
		bt_cSphereShape(const bt_cSphereShape& other);
		bt_cSphereShape& operator=(const bt_cSphereShape& other);

		float mRadius;

        // Bullet
        btCollisionShape* fallShape;
	};

	class bt_cPlaneShape : public iShape
	{
	public:

		bt_cPlaneShape(const glm::vec3& normal, float planeConst);
		
		virtual ~bt_cPlaneShape();

		virtual bool GetPlaneNormal(glm::vec3& normalOut);
		virtual bool GetPlaneConst(float& planeConstOut);

        inline btCollisionShape* getBulletShape() { return this->groundShape; }

	private:
		bt_cPlaneShape();
		bt_cPlaneShape(const bt_cPlaneShape& other);
		bt_cPlaneShape& operator=(const bt_cPlaneShape& other);

		glm::vec3 mNormal;
		float mPlaneConst;

        // Bullet
        btCollisionShape* groundShape;
	};

    class bt_cBoxShape : public iShape
    {
    public:

        bt_cBoxShape(const glm::vec3& halfExtents);

        virtual ~bt_cBoxShape();

        inline btCollisionShape* getBulletShape() { return this->boxShape; }

    private:
        bt_cBoxShape();
        bt_cBoxShape(const bt_cBoxShape& other);
        bt_cBoxShape& operator=(const bt_cBoxShape& other);


        // Bullet
        btCollisionShape* boxShape;
    };

    class bt_cConvexHullShape : public iShape
    {
    public:

        bt_cConvexHullShape(const GLInstanceVertex*, size_t numOfVertices);

        virtual ~bt_cConvexHullShape();

        inline btCollisionShape* getBulletShape() { return this->convexHullShape; }

    private:
        bt_cConvexHullShape();
        bt_cConvexHullShape(const btCollisionShape& other);
        bt_cConvexHullShape& operator=(const btCollisionShape& other);


        // Bullet
        btConvexHullShape* convexHullShape;
    };
}