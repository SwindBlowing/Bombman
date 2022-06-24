#ifndef PHYSICS_H
#define PHYSICS_H

#include <component.h>
#include <gameobject.h>
#include <transform.h>

class Physics : public Component
{
public :
    Physics () ;

    void onAttach() override;
    void onUpdate(float deltaTime) override;

    void setVelocity(float vx, float vy);
    void setVelocity(float vx, float vy, float limit);
    bool isMoving();

private :
    Transform *transform;
    float vx = 0 , vy = 0;
    float moveSpaceLimit = 0, moveSpace = 0;
    bool flag = 0;
} ;

#endif // PHYSICS_H
