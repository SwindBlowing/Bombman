#include "props.h"

Props::Props(int type)
{
    this->type = type;
}

void Props::onAttach()
{
    transform = this->gameObject->getComponent<ImageTransform>();
    assert(transform != nullptr);
    transform->setImage(QString(":/Image/prop%1.png").arg(type));
    transform->setZValue(1);
}

void Props::onUpdate(float deltaTime) {
    if (toDelete) destory(this->gameObject);
}

short Props::getType()
{
    return type;
}

void Props::del()
{
    toDelete = 1;
}
