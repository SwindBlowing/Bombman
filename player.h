#ifndef PLAYER_H
#define PLAYER_H

#include <imagetransform.h>
#include <component.h>
#include <QQueue>

class Player: public Component
{
    struct tmp
    {
        int type;
        float startTime = 0;
    };

public:
    Player(int id, int towards);

    void onAttach() override;
    void onUpdate(float deltaTime) override;
    bool move(int towards);
    bool putBomb();
    void checkAlive();
    void checkProp();
    void updateTransform();
    void getBenefit(int type);
    void finishBenefit(int type);
    void checkBomb();
    void arrived();

    short getX();
    short getY();
    short getPower();
    short getId();
    bool canPutBomb();

    void returnLimit();
    void playDeadAnimate();

    void getCredit(int t);
    int returnCredit();

private:
    const float dx[4] = {0, 1, 0, -1}, dy[4] = {-1, 0, 1, 0};
    ImageTransform *transform = nullptr;
    short id = 0;
    short towards = 0;
    float speed = 100;
    short power = 1;
    short putLimit = 1;
    short canPushBomb = 0;
    short x = 0, y = 0;
    QTimer * moveTimer = nullptr;
    short nowPic = 1;
    bool toDead = 0;

    QQueue<tmp> Q;
    float nowTime = 0;
    int credit = 0;

    float putInterval = 0;
    bool canPutAgain = 1;

    short prop[4] = {0};
};

#endif // PLAYER_H
