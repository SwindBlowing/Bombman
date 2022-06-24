#include "mainwindow.h"
#include "beginscene.h"

#include <QApplication>
#include <mapoperator.h>

MapOperator *opt = new MapOperator();

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow *w = new MainWindow();
    return a.exec();
}
