#include "robot.h"

#include <gameobject.h>
#include <component.h>
#include <transformbuilder.h>
#include <transform.h>
#include <QKeyEvent>
#include <physics.h>
#include <bomb.h>
#include <mapoperator.h>
#include <QTimer>

Robot::Robot()
{

}

void Robot::robotMove()
{
    extern MapOperator *opt;
    QPoint enemy = opt->getEnemy(player->getX(), player->getY());
    int dir = opt->shortestPath(player->getX(), player->getY(), enemy.x(), enemy.y());
    bool flag = opt->willBeBombed(player->getX(), player->getY());
    short val[4] = {0}, minn = 30000, standard = opt->getVal(player->getX(), player->getY(), player->getId());
    for (int i = 0; i < 4; i++) {
        val[i] = opt->getVal(player->getX() + dx[i], player->getY() + dy[i], player->getId());
        if (i == dir && !flag) val[i] -= 50;
        minn = val[i] < minn ? val[i] : minn;
    }
    if ((standard <= minn && !flag) ||
            (val[0] == val[1] && val[1] == val[2] && val[2] == val[3] && val[3] == 20000))
        return ;
    short str = rand() % 4;
    for (int i = 0; i < 4; i++)
        if (val[(str + i) % 4] == minn) {
            dir = (str + i) % 4;
            break;
        }
    player->move(dir);
    return ;
}

bool Robot::robotPutBomb()
{
    extern MapOperator *opt;
    if (this->gameObject->getComponent<Physics>()->isMoving() || !player->canPutBomb())
        return 0;
    if (opt->existSoftWallOrPlayers(player, nextStep))
        return player->putBomb();
    return 0;
}

void Robot::onAttach()
{
    player = this->gameObject->getComponent<Player>();
    assert(player != nullptr);
}

void Robot::onUpdate(float deltaTime)
{
    extern MapOperator *opt;
    player->updateTransform();
    //robotPutBomb();
    if (!this->gameObject->getComponent<Physics>()->isMoving()) {
        QPoint enemy = opt->getEnemy(player->getX(), player->getY());
        nextStep = opt->shortestPath(player->getX(), player->getY(), enemy.x(), enemy.y());
        robotPutBomb();
        robotMove();
    }
}
