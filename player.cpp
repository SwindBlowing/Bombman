#include "player.h"

#include <gameobject.h>
#include <component.h>
#include <transformbuilder.h>
#include <transform.h>
#include <QKeyEvent>
#include <physics.h>
#include <bomb.h>
#include <mapoperator.h>

#include <QTimer>
#include "gamescene.h"

Player::Player(int id, int towards)
{
    Q.clear();
    this->id = id;
    this->towards = towards;
}

void Player::onAttach()
{
    transform = this->gameObject->getComponent<ImageTransform>();
    assert(transform != nullptr);
    transform->setZValue(2);
    x = transform->x() / 40 + 1;
    y = transform->y() / 40 + 1;
    moveTimer = new QTimer(this);
}

bool Player::move(int towards)
{
    if (towards < 0 || towards >= 4) return 0;
    extern MapOperator *opt;
    if (this->gameObject->getComponent<Physics>()->isMoving()) return 0;
    this->towards = towards;
    int tx = (transform->x() + 10) / 40 + dx[towards] + 1, ty = (transform->y() + 10) / 40 + dy[towards] + 1;
    if (!opt->canVisit(tx, ty)) return 0;
    this->gameObject->getComponent<Physics>()->setVelocity(speed * dx[towards], speed * dy[towards], 40);
    moveTimer->start(150);
    nowPic = 0;
    transform->setImage(QString(":/Image/Player%1Type%2%3").arg(id).arg(towards).arg(0));
    connect(moveTimer, &QTimer::timeout, [=](){
        if (!this->gameObject->getComponent<Physics>()->isMoving() || toDead) ;
        else {
            nowPic++;
            if (nowPic > 4) nowPic -= 4;
            transform->setImage(QString(":/Image/Player%1Type%2%3").arg(id).arg(towards).arg(nowPic % 4 + 1));
        }
    });
    return 1;
}

short Player::getPower()
{
    return power;
}

bool Player::putBomb()
{
    extern MapOperator *opt;
    if (putLimit <= 0) return 0;
    putLimit--;
    canPutAgain = 0;
    auto obj = new GameObject();
    ImageTransformBuilder()
            .setPos(transform->pos())
            .setImage(":/Image/bomb1.png")
            .addToGameObject(obj);
    Bomb *nowBomb = new Bomb(this, power);
    obj->addComponent(nowBomb);
    obj->addComponent(new Physics());
    attachGameObject(obj);
    opt->addBombs(obj);
    return 1;
}

void Player::playDeadAnimate()
{
    this->gameObject->playDeadAnimate(transform->x(), transform->y(), id);
}

void Player::onUpdate(float deltaTime)
{
    putInterval += deltaTime * 1000;
    if (putInterval > 300) {
        canPutAgain = 1;
        putInterval = 0;
    }
    nowTime += deltaTime * 1000;
    while (!Q.empty()) {
        tmp now = Q.front();
        if (nowTime - now.startTime > 8000) {
            Q.pop_front();
            finishBenefit(now.type);
        }
        else break;
    }
    if (toDead) {
        playDeadAnimate();
        destory(this->gameObject);
    }
    updateTransform();
    checkAlive();
    checkProp();
    checkBomb();
    if (id == 1) {
        if (getKey(Qt::Key_W)) move(0);
        if (getKey(Qt::Key_D)) move(1);
        if (getKey(Qt::Key_S)) move(2);
        if (getKey(Qt::Key_A)) move(3);
        if (getKeyDown(Qt::Key_Space)) putBomb();
    }
    if (id == 2) {
        if (getKey(Qt::Key_I)) move(0);
        if (getKey(Qt::Key_L)) move(1);
        if (getKey(Qt::Key_K)) move(2);
        if (getKey(Qt::Key_J)) move(3);
        if (getKeyDown(Qt::Key_Enter) || getKeyDown(Qt::Key_Return)) putBomb();
    }
    if (!this->gameObject->getComponent<Physics>()->isMoving())
        transform->setImage(QString(":/Image/Player%1Type%2%3").arg(id).arg(towards).arg(0));
}

void Player::checkAlive()
{
    extern MapOperator *opt;
    int nowx = 1 + (transform->x() + 20) / 40, nowy = 1 + (transform->y() + 20) / 40;
    if (opt->isBombing(nowx, nowy)) {
        opt->detachPlayers(this);
        opt->playDeadSound();
        toDead = 1;
    }
}

void Player::updateTransform()
{
    if (this->gameObject->getComponent<Physics>()->isMoving()) return ;
    transform->setPos(QPointF((int)((transform->x() + 20) / 40) * 40, (int)((transform->y() + 20) / 40) * 40));
}

void Player::checkProp()
{
    extern MapOperator *opt;
    int nowx = 1 + (transform->x() + 20) / 40, nowy = 1 + (transform->y() + 20) / 40;
    if (opt->isProp(nowx, nowy)) {
        getBenefit(opt->propType(nowx, nowy));
        opt->delP(nowx, nowy);
    }
}

void Player::getBenefit(int type)
{
    if (type == 1)
        speed += 50;
    if (type == 2)
        power++;
    if (type == 3)
        putLimit++;
    if (type == 4)
        canPushBomb++;
    prop[type]++;
    if (prop[type] == 1) {
        extern MapOperator *opt;
        opt->dealWithPropsShow(id, type, 1);
    }
    Q.push_back((tmp){type, nowTime});
    getCredit(8);
}

void Player::finishBenefit(int type)
{
    if (type == 1)
        speed -= 50;
    if (type == 2)
        power--;
    if (type == 3)
        putLimit--;
    if (type == 4)
        canPushBomb--;
    prop[type]--;
    if (!prop[type]) {
        extern MapOperator *opt;
        opt->dealWithPropsShow(id, type, 0);
    }
}

void Player::checkBomb()
{
    if (!canPushBomb || !this->gameObject->getComponent<Physics>()->isMoving()) return ;
    extern MapOperator *opt;
    opt->pushBombs(x + dx[towards], y + dy[towards], towards);
}

void Player::arrived()
{
    x += dx[towards];
    y += dy[towards];
}

short Player::getX()
{
    return x;
}

short Player::getY()
{
    return y;
}

bool Player::canPutBomb()
{
    return putLimit > 0;
}

short Player::getId()
{
    return id;
}

void Player::returnLimit()
{
    putLimit++;
}

void Player::getCredit(int t)
{
    credit += t;
}

int Player::returnCredit()
{
    return credit;
}

