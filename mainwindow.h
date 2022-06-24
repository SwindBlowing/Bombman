#ifndef MAINWINDOW_H_
#define MAINWINDOW_H_

#include <gamescene.h>
#include <gameobject.h>
#include <QGraphicsView>
#include <QMainWindow>
#include <grounditem.h>
#include <beginscene.h>
#include <QSoundEffect>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
  Q_OBJECT

public:
  explicit MainWindow(QWidget *parent = nullptr);
  ~MainWindow();


private:
  Ui::MainWindow *ui;
  GameScene *gameScene;
  bool firstShow = 1;
  bool isStoped = 0;
  QSoundEffect *playSound = nullptr;

signals:
  void sceneBack();
};
#endif  // MAINWINDOW_H_
