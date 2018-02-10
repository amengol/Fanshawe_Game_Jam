#pragma once
#include <glm\vec3.hpp>
#include <iShape.h>

namespace nPhysics
{
	class cSphereShape : public iShape
	{
	public:
		cSphereShape(float radius);

		virtual ~cSphereShape();

		virtual bool GetSphereRadius(float& radiusOut);

        inline float getRadius() { return this->mRadius; }

	private:
		cSphereShape();
		cSphereShape(const cSphereShape& other);
		cSphereShape& operator=(const cSphereShape& other);

		float mRadius;
	};

	class cPlaneShape : public iShape
	{
	public:

        enum PlaneType
        {
            FLOOR,
            FRONT,
            BACK,
            LEFT,
            RIGHT
        };

		cPlaneShape(const glm::vec3& normal, float planeConst);
		
		virtual ~cPlaneShape();

        inline PlaneType getPlaneType() { return this->type; }
        inline glm::vec3 getPlanePosition() { return this->mPostiion; }
		virtual bool GetPlaneNormal(glm::vec3& normalOut);
		virtual bool GetPlaneConst(float& planeConstOut);

	private:
		cPlaneShape();
		cPlaneShape(const cPlaneShape& other);
		cPlaneShape& operator=(const cPlaneShape& other);

		glm::vec3 mNormal;
		float mPlaneConst;
        PlaneType type;
        glm::vec3 mPostiion;
	};
}