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
    void changePicPos(double p);

private:
    QString normalImgPath;
    QString pressImgPath;
    bool flag = 0;
    double dy = 5;
signals:

};

#endif // MYPUSHBUTTON_H
