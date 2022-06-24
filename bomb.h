#ifndef BOMB_H
#define BOMB_H

#include <component.h>
#include <gameobject.h>
#include <transform.h>
#include <imagetransform.h>
#include <player.h>

class Bomb: public Component
{
public:
    Bomb(Player *master, int power);

    void onAttach() override;
    void onUpdate(float deltaTime) override;

    QPoint getInitPos();
    void bombing();
    void bombEvent(int x, int y, int power);

    short getX();
    short getY();

    void switchOn();
    void startMove(int towards);
    void move();
    bool canBePushed();
    void updateTransform();

    short getPower();

private:
    const float dx[4] = {0, 1, 0, -1}, dy[4] = {-1, 0, 1, 0};
    ImageTransform *transform = nullptr;
    short power = 1;
    short towards = 0;
    Player *master = nullptr;
    bool toBomb = 0;
    bool moveFlag = 0;
    bool hasBombed = 0;
    bool toDelete = 0;
    float speed = 180;
    int now = 1;
    QTimer *ownTimer = nullptr;

    float nowTime = 0;

};

#endif // BOMB_H
