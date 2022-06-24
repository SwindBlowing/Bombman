#ifndef ROBOT_H
#define ROBOT_H

#include <component.h>
#include <gameobject.h>
#include <transform.h>
#include <imagetransform.h>
#include <player.h>

class Robot: public Component
{
public:
    Robot();

    void onAttach() override;
    void onUpdate(float deltaTime) override;

    void robotMove();
    bool robotPutBomb();

private:
    const int dx[4] = {0, 1, 0, -1}, dy[4] = {-1, 0, 1, 0};
    Player *player = nullptr;
    float interval = 0;
    short nextStep = 0;
};

#endif // ROBOT_H
