#include "physics.h"

#include <imagetransform.h>
#include <player.h>
#include <robot.h>

Physics::Physics(){}

void Physics::onAttach()
{
    transform = this->gameObject->getComponent<ImageTransform>();
    assert(transform != nullptr);
}

void Physics::onUpdate(float deltaTime)
{
    transform->moveBy(vx * deltaTime, vy * deltaTime);
    if (moveSpaceLimit) {
        moveSpace += fabs(vx * deltaTime) + fabs(vy * deltaTime);
        if (moveSpace > moveSpaceLimit) {
            setVelocity(0, 0);
            moveSpaceLimit = 0;
            moveSpace = 0;
            if (this->gameObject->getComponent<Player>() != nullptr)
                this->gameObject->getComponent<Player>()->arrived();
        }
    }
}

void Physics::setVelocity(float vx, float vy)
{
    this->vx = vx;
    this->vy = vy;
    if (vx || vy) flag = 1;
    else flag = 0;
}

void Physics::setVelocity(float vx, float vy, float limit)
{
    this->vx = vx;
    this->vy = vy;
    moveSpaceLimit = limit;
    if (vx || vy) flag = 1;
    else flag = 0;
}

bool Physics::isMoving()
{
    return flag;
}
