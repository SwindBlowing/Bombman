#include "givememoney.h"
#include <gamescene.h>
#include <QGraphicsView>
#include <transformbuilder.h>
#include <mypushbutton.h>

GiveMeMoney::GiveMeMoney(QWidget *parent)
    : QMainWindow{parent}
{
    this->setFixedSize(800, 600);
    this->setWindowTitle("说明");
    this->setWindowIcon(QPixmap(":/Image/icon.png"));
    gameScene = new GameScene(0);
    gameScene->setParent(this);
    auto view = new QGraphicsView(gameScene, this);
    view->setFrameStyle(QFrame::NoFrame);
    view->resize(this->size());
    view->setSceneRect(QRect(0, 0, this->width(), this->height()));

    loadScene(gameScene);
}

void GiveMeMoney::loadScene(GameScene *gameScene)
{
    auto obj = new GameObject();
    ImageTransformBuilder()
            .setPos(QPoint(0, 0))
            .setImage(":/Image/Money.png")
            .addToGameObject(obj);
    gameScene->attachGameObject(obj);

    auto backBtn = new MyPushButton(QString(":/Image/return.png"));
    backBtn->setParent(this);
    backBtn->move(670, 528);
    connect(backBtn, &QPushButton::clicked, [=](){
        emit this->hasReturned();
    });
}
