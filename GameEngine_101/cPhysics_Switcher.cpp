#include "cPhysics_Switcher.h"
#include "cGameObject.h"

extern std::vector<cGameObject*>  g_vecGameObjects;


void cPhysics_Switcher::SetBullet()
{
    if (gPhysicsEngine == BULLET)
        return;

    for (size_t i = 0; i < g_vecGameObjects.size(); i++)
    {
        cGameObject* GO = g_vecGameObjects[i];

        if (GO->rigidBody != NULL && GO->bt_rigidBody != NULL)
        {
            glm::vec3 position(0.0f);
            GO->rigidBody->GetPostion(position);
            GO->bt_rigidBody->SetPosition(position);

            glm::vec3 velocity(0.0f);
            GO->rigidBody->GetVelocity(velocity);
            GO->bt_rigidBody->SetVelocity(velocity);

            glm::mat4 orientation(1.0f);
            GO->rigidBody->GetMatOrientation(orientation);
            GO->bt_rigidBody->SetMatOrientation(orientation);
        }
    }
    gPhysicsEngine = BULLET;
}

void cPhysics_Switcher::SetSuperDuper()
{
    if (gPhysicsEngine == SUPERDUPER)
        return;

    for (size_t i = 0; i < g_vecGameObjects.size(); i++)
    {
        cGameObject* GO = g_vecGameObjects[i];

        if (GO->rigidBody != NULL && GO->bt_rigidBody != NULL)
        {
            glm::vec3 position(0.0f);
            GO->bt_rigidBody->GetPostion(position);
            GO->rigidBody->SetPosition(position);

            glm::vec3 velocity(0.0f);
            GO->bt_rigidBody->GetVelocity(velocity);
            GO->rigidBody->SetVelocity(velocity);

            glm::mat4 orientation(1.0f);
            GO->bt_rigidBody->GetMatOrientation(orientation);
            GO->rigidBody->SetMatOrientation(orientation);
        }
    }
    gPhysicsEngine = SUPERDUPER;
}
