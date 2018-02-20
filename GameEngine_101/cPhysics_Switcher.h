#ifndef _PHYSICS_SWITCHER_HG_
#define _PHYSICS_SWITCHER_HG_

class cPhysics_Switcher
{
public:

    cPhysics_Switcher() : gPhysicsEngine(SUPERDUPER) { }

    enum ActivePhysicsEngine
    {
        SUPERDUPER,
        BULLET
    };

    // Changes the main Physics Engine to Bullet
    void SetBullet();

    // Changes the main Physics Engine to SuperDuper
    void SetSuperDuper();

    ActivePhysicsEngine gPhysicsEngine; // The active Physics Engine

};

#endif // !_PHYSICS_SWITCHER_HG_

