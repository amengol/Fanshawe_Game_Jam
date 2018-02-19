#pragma once
#include <glm\vec3.hpp>
#include <iShape.h>

namespace nPhysics
{
	class bt_cSphereShape : public iShape
	{
	public:
		bt_cSphereShape(float radius);

		virtual ~bt_cSphereShape();

		virtual bool GetSphereRadius(float& radiusOut);

        inline float getRadius() { return this->mRadius; }

	private:
		bt_cSphereShape();
		bt_cSphereShape(const bt_cSphereShape& other);
		bt_cSphereShape& operator=(const bt_cSphereShape& other);

		float mRadius;
	};

	class bt_cPlaneShape : public iShape
	{
	public:

		bt_cPlaneShape(const glm::vec3& normal, float planeConst);
		
		virtual ~bt_cPlaneShape();

        virtual bool getPlaneType(PlaneType& type);
        virtual bool setGameType(const PlaneType& type);

		virtual bool GetPlaneNormal(glm::vec3& normalOut);
		virtual bool GetPlaneConst(float& planeConstOut);

	private:
		bt_cPlaneShape();
		bt_cPlaneShape(const bt_cPlaneShape& other);
		bt_cPlaneShape& operator=(const bt_cPlaneShape& other);

		glm::vec3 mNormal;
		float mPlaneConst;
        PlaneType type;
	};

    class bt_cCubeShape : public iShape
    {
    public:
        bt_cCubeShape(float size);
        virtual ~bt_cCubeShape();

    private:
        bt_cCubeShape();
        float cubeSize;
    };
}