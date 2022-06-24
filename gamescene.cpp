#include "gamescene.h"

#include <QKeyEvent>
#include <QTimer>
#include <utility>
#include <qdebug.h>

#include "gameobject.h"
#include "transform.h"
#include "imagetransform.h"
#include "grounditem.h"
#include "physics.h"
#include "transformbuilder.h"
#include "mapoperator.h"

#include <QGraphicsItem>
#include <QSoundEffect>

GameScene::GameScene(bool flag) {
  isMainScene = flag;
  updateTimer = new QTimer(this);
  updateTimer->setInterval(1000 / 60);
  connect(updateTimer, &QTimer::timeout, this, &GameScene::onUpdate);
  updateTimer->start();

  if (isMainScene) {
      totalTimer = new QTimer(this);
      totalTimer->setInterval(120000);
      connect(totalTimer, &QTimer::timeout, this, &GameScene::gameToOver);
      totalTimer->start();

      for (int i = 0; i < 3; i++) {
           p[i] = new QGraphicsTextItem();
           if (i == 0) p[i]->setPlainText(QString("2"));
           else p[i]->setPlainText(QString("0"));
           p[i]->setTextWidth(300);
           if (i == 0) p[i]->setPos(890,360);
           else if (i == 1) p[i]->setPos(940, 360);
           else p[i]->setPos(970, 360);
           p[i]->setScale(4);
           this->addItem(p[i]);
       }
       auto paint = new QGraphicsTextItem();
       paint->setPlainText(QString(":"));
       paint->setTextWidth(300);
       paint->setPos(923, 356);
       paint->setScale(4);
       this->addItem(paint);
  }

}

GameScene::~GameScene() {
  updateTimer->stop();
  for (auto gameObject : gameObjects) {
    gameObject->onDetach();
    gameObject->setParentGameScene(nullptr);
  }
}

void GameScene::attachGameObject(GameObject *gameObject) {
  gameObjectsToAttach.emplace_back(gameObject);
}
void GameScene::detachGameObject(GameObject *gameObject) {
  gameObjectsToDetach.emplace_back(gameObject);
}
GameObject *GameScene::getGameObject(const char *name) {
  for (auto gameObject : gameObjects) {
    if (gameObject->objectName() == name) return gameObject;
  }
  return nullptr;
}

void GameScene::onUpdate() {

    if (toOver) {
        toOver = 0;
        gameOver();
    }

    if (isMainScene) {
        int now = totalTimer->remainingTime() / 1000;
        p[0]->setPlainText(QString("%1").arg((int)(now / 60)));
        p[1]->setPlainText(QString("%1").arg((int)((now % 60) / 10)));
        p[2]->setPlainText(QString("%1").arg((int)((now % 60) % 10)));
    }

    //qDebug() << totalTimer->remainingTime();
  // Do firstUpdate() for game objects
  for (auto gameObject : gameObjectsAttachedOnLastUpdate) {
    gameObject->onFirstUpdate();
  }

  // Do update() for game objects
  for (auto gameObject : gameObjects) {
    gameObject->onUpdate(updateTimer->interval() / 1000.0);
  }

  // Do detach() for game objects
  for (auto gameObject : gameObjectsToDetach) {
    gameObject->onDetach();
    gameObject->setParentGameScene(nullptr);
    this->gameObjects.removeAll(gameObject);
    auto tf = gameObject->getComponent<Transform>();
    if (tf != nullptr) this->removeItem(tf);
  }
  gameObjectsToDetach.clear();

  // Do attach() for game objects
  for (auto gameObject : gameObjectsToAttach) {
    gameObject->setParentGameScene(this);
    this->gameObjects.append(gameObject);
    auto tf = gameObject->getComponent<Transform>();
    if (tf != nullptr) this->addItem(tf);
    gameObject->onAttach();
  }
  std::swap(gameObjectsToAttach, gameObjectsAttachedOnLastUpdate);
  gameObjectsToAttach.clear();

  // Keyboard input
  keyDownTable.clear();
  keyUpTable.clear();
}

void GameScene::keyPressEvent(QKeyEvent *ev) {
  if (ev->isAutoRepeat()) return;
  keyTable[ev->key()] = true;
  keyDownTable[ev->key()] = true;
  QGraphicsScene::keyPressEvent(ev);
}
void GameScene::keyReleaseEvent(QKeyEvent *ev) {
  if (ev->isAutoRepeat()) return;
  keyTable[ev->key()] = false;
  keyUpTable[ev->key()] = true;
  QGraphicsScene::keyReleaseEvent(ev);
}
void GameScene::focusOutEvent(QFocusEvent *ev) {
  keyTable.clear();
  keyDownTable.clear();
  QGraphicsScene::focusOutEvent(ev);
}

void GameScene::pause()
{
    updateTimer->stop();
    if (totalTimer->remainingTime() > 0)
        leftTime = totalTimer->remainingTime();
    totalTimer->stop();
}

void GameScene::restart()
{
    updateTimer->start();
    totalTimer->start(leftTime);
}

void GameScene::playDeadAnimate(int x, int y, int id)
{
    QTimer *timer = new QTimer(this);
    auto obj = new GameObject();
    auto transform = new ImageTransform();
    transform->setImage(QString(":/Image/Player%1Dead1.png").arg(id));
    transform->setPos(QPoint(x, y));
    transform->setZValue(10);
    obj->addComponent(transform);
    auto phy = new Physics();
    obj->addComponent(phy);
    this->attachGameObject(obj);
    timer->start(1000);
    connect(timer, &QTimer::timeout, [=](){
        timer->stop();
        transform->setImage(QString(":/Image/Player%1Dead2.png").arg(id));
        phy->setVelocity(0, -250, 600);
        timer->start(4000);
    });
}

void GameScene::gameOver()
{

    if (overed) return ;
    emit this->gameOvered();
    extern MapOperator *opt;
    overed = 1;

    totalTimer->stop();
    updateTimer->deleteLater();
    auto transform = new ImageTransform();
    transform->setImage(QString(":/Image/Player%1Win.png").arg(opt->winner()));

    auto endSound = new QSoundEffect(this);

    if (opt->winner() != 5) {
        transform->setPos(QPoint(300, 0));
        endSound->setSource(QUrl::fromLocalFile(QString(":/new/Sound/win.wav")));
    }
    else {
        transform->setPos(QPoint(300, 80));
        endSound->setSource(QUrl::fromLocalFile(QString(":/new/Sound/Balance.wav")));
    }
    endSound->play();
    transform->setZValue(11);
    addItem(transform);
}

bool GameScene::isOvered()
{
    return overed;
}

void GameScene::gameToOver()
{
    toOver = 1;
}
