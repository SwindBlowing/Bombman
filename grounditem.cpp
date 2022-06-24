#include "grounditem.h"

#include <transformbuilder.h>
#include <transform.h>
#include <QString>
#include <QTimer>

GroundItem::GroundItem(){}

void GroundItem::onAttach()
{
    transform = this->gameObject->getComponent<ImageTransform>();
    assert(transform != nullptr);
    transform->setImage(QString(":/Image/GroundType%1.png").arg(type));
    transform->setZValue(2);
    timer = new QTimer(this);
}

void GroundItem::onUpdate(float deltaTime)
{
    if (!isBombing) transform->setImage(QString(":/Image/GroundType%1.png").arg(type));
}

short GroundItem::getType()
{
    return type;
}

bool GroundItem::canVisit()
{
    return type == 1;
}

void GroundItem::setType(int type)
{
    this->type = type;
}

void GroundItem::del()
{
    this->gameObject->deleteLater();
}

bool GroundItem::receiveBombing(QString st)
{
    isBombing++;
    transform->setImage(st);
    if (type == 2) {
        type = 1;
        return 1;
    }
    return 0;
}

void GroundItem::endBombing()
{
    isBombing--;
}
