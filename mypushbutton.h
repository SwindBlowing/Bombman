#ifndef MYPUSHBUTTON_H
#define MYPUSHBUTTON_H

#include <QPushButton>

class MyPushButton : public QPushButton
{
public:
    //explicit MyPushButton(QWidget *parent = nullptr);

    MyPushButton(QString normalImg, QString pressImg = "");
    void changePic();
    void changeBackPic();

private:
    QString normalImgPath;
    QString pressImgPath;

signals:

};

#endif // MYPUSHBUTTON_H
