#ifndef GROUNDITEM_H
#define GROUNDITEM_H

#include <gameobject.h>
#include <component.h>
#include <transform.h>
#include <imagetransform.h>

class GroundItem: public Component
{
public:
    GroundItem();

    void onAttach() override;
    void onUpdate(float deltaTime) override;

    short getType();
    bool canVisit();
    void setType(int type);
    void del();

    bool receiveBombing(QString st);
    void endBombing();

private:
    ImageTransform *transform = nullptr;
    short type = 0;
    short isBombing = 0;
    QTimer *timer = nullptr;
};

#endif // GROUNDITEM_H
