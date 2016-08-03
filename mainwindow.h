#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <api.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void moleculeInfo(QString SMILE);

private:
    Ui::MainWindow *ui;
    API* api;
};

#endif // MAINWINDOW_H
