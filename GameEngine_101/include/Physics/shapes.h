#pragma once
#include <glm\game_math.h>
#include <Physics\Interfaces\iShape.h>

namespace nPhysics
{
	class cSphereShape : public iShape
	{
	public:
		cSphereShape(float radius);

		virtual ~cSphereShape();

		virtual bool GetSphereRadius(float& radiusOut);

	private:
		cSphereShape();
		cSphereShape(const cSphereShape& other);
		cSphereShape& operator=(const cSphereShape& other);

		float mRadius;
	};

	class cPlaneShape : public iShape
	{
	public:
		cPlaneShape(const glm::vec3& normal, float planeConst);
		
		virtual ~cPlaneShape();

		virtual bool GetPlaneNormal(glm::vec3& normalOut);
		virtual bool GetPlaneConst(float& planeConstOut);

	private:
		cPlaneShape();
		cPlaneShape(const cPlaneShape& other);
		cPlaneShape& operator=(const cPlaneShape& other);

		glm::vec3 mNormal;
		float mPlaneConst;
	};
}