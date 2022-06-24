#include "mainwindow.h"

#include <gameobject.h>
#include <imagetransform.h>
#include <transformbuilder.h>
#include <grounditem.h>
#include <QDebug>
#include <player.h>
#include <physics.h>
#include <mapoperator.h>
#include <props.h>
#include <robot.h>
#include <QList>
#include <mypushbutton.h>
#include <beginscene.h>
#include <QTimer>

#include "./ui_mainwindow.h"

bool bornPlace(int x, int y)
{
    if (x + y <= 1) return true;
    return false;
}


void buildMap(GameScene *gameScene)
{
    extern MapOperator *opt;
    srand(time(0));
    for (int i = 0; i < 20; i++)
        for (int j = 0; j < 15; j++) {
            auto obj = new GameObject();
            auto transform = new ImageTransform();
            transform->setPos(QPoint(i * 40, j * 40));
            transform->setImage(QString(":/Image/Ground%1.png").arg((i * 15 + j) % 2 + 1));
            transform->setZValue(0.5);
            obj->addComponent(transform);
            gameScene->attachGameObject(obj);
        }
    QList<GameObject *> Q, P;
    do {
        for (auto object : P) object->deleteLater();
        for (auto object : Q) object->deleteLater();
        P.clear(); Q.clear();
        for (int i = 0; i < 20; i++)
            for (int j = 0; j < 15; j++) {
                auto obj = new GameObject();
                auto gnd = new GroundItem();
                int randNum = rand() % 10, type = 0; // 1草地 2软墙 3硬墙
                if (bornPlace(i, j) || bornPlace(19 - i, 14 - j) || bornPlace(19 - i, j) || bornPlace(i, 14 - j))
                     type = 1;
                else if (randNum < 3) type = 1;
                else if (randNum < 7) {
                    type = 2;
                    int randNum2 = rand() % 4;
                    if (!randNum2) {
                        randNum2 = rand() % 4 + 1; //1 speed up, 2 power up, 3 num up, 4 push bomb.
                        auto obj2 = new GameObject();
                        auto prop = new Props(randNum2);
                        ImageTransformBuilder()
                                .setPos(QPoint(i * 40, j * 40))
                                .addToGameObject(obj2);
                        obj2->addComponent(prop);
                        P.emplace_back(obj2);
                    }
                }
                else type = 3;
                gnd->setType(type);
                ImageTransformBuilder()
                        .setPos(QPoint(i * 40, j * 40))
                        .addToGameObject(obj);
                obj->addComponent(gnd);
                Q.emplace_back(obj);
                opt->getInit(i + 1, j + 1, type);
            }
    }while (!opt->checkMap());
    for (auto object : P) {
        opt->addProps((object->getComponent<ImageTransform>()->x() + 20) / 40 + 1,
                      (object->getComponent<ImageTransform>()->y() + 20) / 40 + 1,
                      object->getComponent<Props>());
        gameScene->attachGameObject(object);
    }
    for (auto object : Q) {
        opt->addItems((object->getComponent<ImageTransform>()->x() + 20) / 40 + 1,
                      (object->getComponent<ImageTransform>()->y() + 20) / 40 + 1,
                      object->getComponent<GroundItem>());
        gameScene->attachGameObject(object);
    }
}

void creatPlayer(GameScene *gameScene, int id)
{
    extern MapOperator *opt;
    auto obj = new GameObject();
    auto player = new Player(id + 1, 2);
    ImageTransformBuilder()
            .setPos(QPoint((id & 1) * 19 * 40, (id >> 1) * 14 * 40))
            .setImage(":/Image/Player1Type00.png")
            .addToGameObject(obj);
    obj->addComponent(player);
    obj->addComponent(new Physics());
    if (id == 2 || id == 3) obj->addComponent(new Robot());
    gameScene->attachGameObject(obj);
    opt->addPlayers(player);
}

void loadScene(GameScene *gameScene) {

    const float zoneX[5] = {0, 870, 990, 870, 990}, zoneY[5] = {0, 195, 195, 305, 305};

    extern MapOperator *opt;
    auto obj = new GameObject();
    obj->addComponent(opt);
    gameScene->attachGameObject(obj);

    for (int i = 0; i < 4; i++)
        gameScene->addItem(opt->paint[i]);
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++) {
            auto obj = new GameObject();
            auto transform = new ImageTransform();
            transform->setPos(zoneX[i + 1] - 45 + j * 30, zoneY[i + 1] + 30);
            transform->setImage(QString(":/Image/GroundType1.png"));
            transform->setZValue(1000);
            obj->addComponent(transform);
            opt->setPlayerProps(i, j, obj);
            gameScene->attachGameObject(obj);
        }

    auto obj2 = new GameObject();
    auto transform = new ImageTransform();
    transform->setImage(":/Image/board.png");
    transform->setPos(QPoint(800, 0));
    transform->setZValue(-10);
    obj2->addComponent(transform);
    gameScene->attachGameObject(obj2);

    buildMap(gameScene);
    for (int i = 0; i < 4; i++)
        creatPlayer(gameScene, i);
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);
  this->setFixedSize(1100, 600);
  this->setWindowIcon(QPixmap(":/Image/icon.png"));

  gameScene = new GameScene(1);
  gameScene->setParent(this);

  auto view = new QGraphicsView(gameScene, this);
  view->setFrameStyle(QFrame::NoFrame);
  view->resize(this->size());
  view->setSceneRect(QRect(0, 0, this->width(), this->height()));

  playSound = new QSoundEffect(this);
  playSound->setSource(QUrl::fromLocalFile(QString(":/new/Sound/Play.wav")));

  loadScene(gameScene);
  gameScene->pause();

  connect(gameScene, &GameScene::gameOvered, [=](){
      playSound->stop();
  });
  auto beginScene = new BeginScene();
  beginScene->show();
  connect(beginScene, &BeginScene::gameStart, [=](){
      beginScene->hide();
      if (!gameScene->isOvered()) {
          playSound->setLoopCount(800);
          playSound->play();
      }
      if (firstShow) {
          gameScene->restart();
          isStoped = 0;
          firstShow = 0;
      }
      this->show();
  });

  auto pauseBtn = new MyPushButton(QString(":/Image/pause.png"), QString(":/Image/continue.png"));
  pauseBtn->setParent(this);
  pauseBtn->move(820, 460);
  connect(pauseBtn, &QPushButton::clicked, [=](){
      pauseBtn->changePicPos(1);
      QTimer::singleShot(80, this, [=](){
          pauseBtn->changePicPos(-1);
      });
      QTimer::singleShot(150, this, [=](){
          if (gameScene->isOvered()) ;
          else if (!isStoped) {
              gameScene->pause();
              isStoped = 1;
              pauseBtn->changePic();
          }
          else {
              gameScene->restart();
              isStoped = 0;
              pauseBtn->changeBackPic();
          }
      });
  });

  auto backBtn = new MyPushButton(QString(":/Image/return.png"));
  backBtn->setParent(this);
  backBtn->move(970, 460);
  connect(backBtn, &QPushButton::clicked, [=](){
      backBtn->changePicPos(1);
      QTimer::singleShot(80, this, [=](){
          backBtn->changePicPos(-1);
      });
      QTimer::singleShot(150, this, [=](){
          playSound->stop();
          pauseBtn->changePic();
          if (!gameScene->isOvered())
              gameScene->pause();
          isStoped = 1;
          this->hide();
          beginScene->show();
          beginScene->playSound();
      });
  });
}


MainWindow::~MainWindow() { delete ui; }
