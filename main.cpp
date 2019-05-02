#include "mainwindow.h"
#include <QApplication>
#include <QScreen>
#include "androidhelper.h"
#include <qtimer.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    AndroidHelper helper;
    MainWindow w;
    w.show();

    //force screen rotation to solve freezing during camera initializon and
    //fix periodictables resizing
    //TODO find better solution
    helper.setScreenOrientation(11);
    QTimer::singleShot(500,&helper,SLOT(setScreenOrientation()));

    return a.exec();
}
