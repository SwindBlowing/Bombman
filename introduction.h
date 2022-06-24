#ifndef INTRODUCTION_H
#define INTRODUCTION_H

#include <QMainWindow>
#include <gamescene.h>

class Introduction : public QMainWindow
{
    Q_OBJECT
public:
    explicit Introduction(QWidget *parent = nullptr);

    void loadScene(GameScene *gameScene);

private:
    GameScene *gameScene;

signals:

    void hasReturned();
};

#endif // INTRODUCTION_H
