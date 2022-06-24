#ifndef BEGINSCENE_H
#define BEGINSCENE_H

#include <QMainWindow>
#include <gamescene.h>
#include <QGraphicsView>
#include <QSoundEffect>

class BeginScene : public QMainWindow
{
    Q_OBJECT
public:
    explicit BeginScene(QWidget *parent = nullptr);

    void loadScene(GameScene *gameScene);

    void playSound();

private:
    GameScene *gameScene;
    QSoundEffect *startSound = nullptr;

signals:
    void gameStart();

};

#endif // BEGINSCENE_H
