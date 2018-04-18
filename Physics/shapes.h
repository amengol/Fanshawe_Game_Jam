#pragma once
#include <glm\vec3.hpp>
#include <iShape.h>
#include <btBulletDynamicsCommon.h>

namespace nPhysics
{
    struct GLInstanceVertex;

	class cSphereShape : public iShape
	{
	public:
		cSphereShape(float radius);

		virtual ~cSphereShape();

		virtual bool GetSphereRadius(float& radiusOut);

        inline float getRadius() { return this->mRadius; }

        inline btCollisionShape* getBulletShape() { return this->fallShape; }

	private:
		cSphereShape();
		cSphereShape(const cSphereShape& other);
		cSphereShape& operator=(const cSphereShape& other);

		float mRadius;

        // Bullet
        btCollisionShape* fallShape;
	};

	class cPlaneShape : public iShape
	{
	public:

		cPlaneShape(const glm::vec3& normal, float planeConst);
		
		virtual ~cPlaneShape();

		virtual bool GetPlaneNormal(glm::vec3& normalOut);
		virtual bool GetPlaneConst(float& planeConstOut);

        inline btCollisionShape* getBulletShape() { return this->groundShape; }

	private:
		cPlaneShape();
		cPlaneShape(const cPlaneShape& other);
		cPlaneShape& operator=(const cPlaneShape& other);

		glm::vec3 mNormal;
		float mPlaneConst;

        // Bullet
        btCollisionShape* groundShape;
	};

    class cBoxShape : public iShape
    {
    public:

        cBoxShape(const glm::vec3& halfExtents);

        virtual ~cBoxShape();

        inline btCollisionShape* getBulletShape() { return this->boxShape; }

    private:
        cBoxShape();
        cBoxShape(const cBoxShape& other);
        cBoxShape& operator=(const cBoxShape& other);


        // Bullet
        btCollisionShape* boxShape;
    };

    class cConvexHullShape : public iShape
    {
    public:

        cConvexHullShape(const GLInstanceVertex*, size_t numOfVertices);

        virtual ~cConvexHullShape();

        inline btCollisionShape* getBulletShape() { return this->convexHullShape; }

    private:
        cConvexHullShape();
        cConvexHullShape(const btCollisionShape& other);
        cConvexHullShape& operator=(const btCollisionShape& other);


        // Bullet
        btConvexHullShape* convexHullShape;
    };

    class cCapsuleShape : public iShape
    {
    public:

        cCapsuleShape(float radius, float height);

        virtual ~cCapsuleShape();

        inline btCollisionShape* getBulletShape() { return this->capmsuleShape; }

    private:
        cCapsuleShape();
        cCapsuleShape(const btCollisionShape& other);
        cCapsuleShape& operator=(const btCollisionShape& other);


        // Bullet
        btCapsuleShape* capmsuleShape;
    };
}