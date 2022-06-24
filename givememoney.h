#ifndef GIVEMEMONEY_H
#define GIVEMEMONEY_H

#include <QMainWindow>
#include <gamescene.h>

class GiveMeMoney : public QMainWindow
{
    Q_OBJECT
public:
    explicit GiveMeMoney(QWidget *parent = nullptr);

    void loadScene(GameScene *gameScene);

private:
    GameScene *gameScene;

signals:

    void hasReturned();

};

#endif // GIVEMEMONEY_H
