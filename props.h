#ifndef PROPS_H
#define PROPS_H

#include <component.h>
#include <gameobject.h>
#include <transform.h>
#include <imagetransform.h>

class Props: public Component
{
public:
    Props(int type);

    void onAttach() override;
    void onUpdate(float deltaTime) override;

    short getType();
    void del();

private:
    ImageTransform *transform = nullptr;
    short type = 0;
    bool toDelete = 0;
};

#endif // PROPS_H
