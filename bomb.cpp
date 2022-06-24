#include "bomb.h"

#include <component.h>
#include <gameobject.h>
#include <transform.h>
#include <imagetransform.h>
#include <transformbuilder.h>
#include <mapoperator.h>
#include <player.h>
#include <physics.h>

#include <QTimer>
#include <QSoundEffect>

Bomb::Bomb(Player *master, int power)
{
    this->master = master;
    this->power = power;
}

QPoint Bomb::getInitPos()
{
    return QPoint((int)((transform->x() + 20) / 40) * 40, (int)((transform->y() + 20) / 40) * 40);
}

void Bomb::bombing()
{
    auto bombSound = new QSoundEffect(this);
    bombSound->setSource(QUrl::fromLocalFile(QString(":/new/Sound/Bombing.wav")));
    bombSound->play();
    int nowx = transform->x() / 40 + 1, nowy = transform->y() / 40 + 1;
    transform->setImage(":/Image/GroundType1.png");
    extern MapOperator *opt;
    QList<QPoint> Q;
    Q.clear();
    assert(Q.empty());
    opt->detachBombs(this->gameObject);
    opt->startBombing(nowx, nowy, QString(":/Image/waveCenter.png"));
    Q.emplace_back(QPoint(nowx, nowy));
    for (int k = 0; k < 4; k++)
        for (int i = 1; i <= power; i++)
            if (!opt->cannotBomb(nowx + i * dx[k], nowy + i * dy[k])) {
                if (i == power || opt->cannotBomb(nowx + (i + 1) * dx[k], nowy + (i + 1) * dy[k]))
                    master->getCredit(2 * opt->startBombing(nowx + i * dx[k], nowy + i * dy[k], QString(":/Image/waveEnd%1").arg(k)));
                else master->getCredit(2 * opt->startBombing(nowx + i * dx[k], nowy + i * dy[k], QString(":/Image/wave%1").arg(k)));
                Q.emplace_back(QPoint(nowx + i * dx[k], nowy + i * dy[k]));
            }
            else break;
}


void Bomb::onAttach()
{
    transform = this->gameObject->getComponent<ImageTransform>();
    assert(transform != nullptr);
    transform->setZValue(1);
    transform->setPos(getInitPos());
    ownTimer = new QTimer(this);
    ownTimer->start(250);
    connect(ownTimer, &QTimer::timeout, [=](){
        if (hasBombed) ownTimer->stop();
        else {
            now++; now %= 3;
            transform->setImage(QString(":/Image/bomb%1.png").arg(now));
        }
    });
}

short Bomb::getX()
{

    return (short)((transform->x() + 20) / 40) + 1;
}

short Bomb::getY()
{
    return (short)((transform->y() + 20) / 40) + 1;
}

void Bomb::switchOn()
{
    toBomb = 1;
    master->returnLimit();
}

void Bomb::startMove(int towards)
{
    moveFlag = 1;
    this->towards = towards;
}

void Bomb::onUpdate(float deltaTime)
{
    nowTime += deltaTime * 1000;
    if (nowTime > 2500 && !toBomb) switchOn();
    if (nowTime > 3300 && hasBombed) toDelete = 1;
    if (toDelete) destory(this->gameObject);
    updateTransform();
    if (toBomb && !hasBombed && !this->gameObject->getComponent<Physics>()->isMoving()) {
        bombing();
        hasBombed = 1;
    }
    if (moveFlag && !toBomb && !hasBombed) move();
}

void Bomb::move()
{
    extern MapOperator *opt;
    if (this->gameObject->getComponent<Physics>()->isMoving()) return ;
    int tx = transform->x() / 40 + dx[towards] + 1, ty = transform->y() / 40 + dy[towards] + 1;
    if (!opt->canVisit(tx, ty)) {
        moveFlag = 0;
        return ;
    }
    this->gameObject->getComponent<Physics>()->setVelocity(speed * dx[towards], speed * dy[towards], 40);
}

bool Bomb::canBePushed()
{
    return !moveFlag;
}

void Bomb::updateTransform()
{
    if (this->gameObject->getComponent<Physics>()->isMoving()) return ;
    transform->setPos(QPointF((int)((transform->x() + 20) / 40) * 40, (int)((transform->y() + 20) / 40) * 40));
}

short Bomb::getPower()
{
    return power;
}
