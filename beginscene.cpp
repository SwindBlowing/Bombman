#include "beginscene.h"
#include "transformbuilder.h"
#include "mypushbutton.h"
#include "mainwindow.h"
#include "mapoperator.h"
#include "introduction.h"
#include "givememoney.h"

#include <QSoundEffect>>

void BeginScene::playSound()
{
    startSound->setLoopCount(800);
    startSound->play();
}

BeginScene::BeginScene(QWidget *parent)
    : QMainWindow{parent}
{
    this->setFixedSize(800, 600);
    this->setWindowTitle("炸弹人");
    this->setWindowIcon(QPixmap(":/Image/icon.png"));
    gameScene = new GameScene(0);
    gameScene->setParent(this);
    auto view = new QGraphicsView(gameScene, this);
    view->setFrameStyle(QFrame::NoFrame);
    view->resize(this->size());
    view->setSceneRect(QRect(0, 0, this->width(), this->height()));

    loadScene(gameScene);
}

void BeginScene::loadScene(GameScene *gameScene)
{
    auto obj = new GameObject();
    ImageTransformBuilder()
            .setPos(QPoint(0, 0))
            .setImage(":/Image/BeginBackground.png")
            .addToGameObject(obj);
    gameScene->attachGameObject(obj);

    startSound = new QSoundEffect(this);
    startSound->setSource(QUrl::fromLocalFile(QString(":/new/Sound/Begin.wav")));
    playSound();

    auto startBtn = new MyPushButton(QString(":/Image/startGame.png"), QString(":/Image/continue_game.png"));
    startBtn->setParent(this);
    startBtn->move(this->width() / 2 - 108, this->height() / 2 - 30);
    connect(startBtn, &MyPushButton::clicked, [=](){
        startSound->stop();
        emit this->gameStart();
        startBtn->changePic();
    });

    auto intro = new Introduction();
    connect(intro, &Introduction::hasReturned, [=](){
        intro->hide();
        this->show();
    });

    auto money = new GiveMeMoney();
    connect(money, &GiveMeMoney::hasReturned, [=](){
        money->hide();
        this->show();
    });

    auto helpBtn = new MyPushButton(QString(":/Image/help.png"));
    helpBtn->setParent(this);
    helpBtn->move(this->width() / 2 - 108, this->height() / 2 - 30 + 90);
    connect(helpBtn, &MyPushButton::clicked, [=](){
        this->close();
        intro->show();
    });

    auto quitBtn = new MyPushButton(QString(":/Image/quitGame.png"));
    quitBtn->setParent(this);
    quitBtn->move(this->width() / 2 - 108, this->height() / 2 - 30 + 180);
    connect(quitBtn, &MyPushButton::clicked, [=](){
        this->close();
    });

    auto moneyBtn = new MyPushButton(QString(":/Image/moneyBtn.png"));
    moneyBtn->setParent(this);
    moneyBtn->move(this->width() / 2 + 220, this->height() / 2 - 30 + 90 + 70);
    connect(moneyBtn, &MyPushButton::clicked, [=](){
        this->close();
        money->show();
    });
}
