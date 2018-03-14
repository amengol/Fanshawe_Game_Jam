#pragma once
#include <iSoftBody.h>
#include <iForm.h>

namespace nPhysics
{
	class cPhysicsWorld;
	class cSoftBody : public iSoftBody
	{
	public:
        cSoftBody(iForm* form) { mForm = form; }
		virtual ~cSoftBody() { }

        virtual iForm* GetForm() { return mForm; }

        // Those would manipulate the main position of the soft body
        // Not using them for now (declared to satisfy iCollisionBody)
        virtual void GetTransform(glm::mat4& transformOut) { return; }
        virtual void GetPostion(glm::vec3& positionOut) { return; }
        virtual void GetRotation(glm::vec3& rotationOut) { return; }
        virtual void GetMatOrientation(glm::mat4& orientationOut) { return; }
        virtual void GetVelocity(glm::vec3& velocity) { return; }
        virtual void SetPosition(glm::vec3& positionIn) { return; }
        virtual void SetRotation(glm::quat& rotationIn) { return; }
        virtual void SetMatOrientation(const glm::mat4& orientationIn) { return; }
        virtual void SetVelocity(const glm::vec3& velocity) { return; }
        virtual void SetVelocityLocal(const glm::vec3& velocity) { return; }
        virtual void rotateX(float degrees) { return; }
        virtual void rotateY(float degrees) { return; }
        virtual void rotateZ(float degrees) { return; }

	private:
		friend class cPhysicsWorld;
        iForm* mForm;
	};
}