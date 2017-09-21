#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->graphicsView,SIGNAL(molInf(QString)),this,SLOT(moleculeInfo(QString)));
    connect(ui->moleculeInfoView,SIGNAL(toEditor(int)),ui->stackedWidget,SLOT(setCurrentIndex(int)));
    connect(ui->periodicTable,SIGNAL(elementChosen(QString)),ui->graphicsView->getScene(),SLOT(setElement(QString)));

    connect(ui->graphicsView,SIGNAL(elements(int)),ui->stackedWidget,SLOT(setCurrentIndex(int)));
    connect(ui->periodicTable,SIGNAL(back(int)),ui->stackedWidget,SLOT(setCurrentIndex(int)));
    connect(ui->periodicTable,SIGNAL(elementChosen(QString)),ui->graphicsView,SLOT(setElement(QString)));
    connect(ui->graphicsView,SIGNAL(camera(int)),ui->stackedWidget,SLOT(setCurrentIndex(int)));
    connect(ui->imageCapture,SIGNAL(back(int)),ui->stackedWidget,SLOT(setCurrentIndex(int)));
  //  connect(ui->graphicsView,SIGNAL(camera(int)),ui->imageCapture,SLOT(img()));
    connect(ui->imageCapture,SIGNAL(image(QImage)),ui->graphicsView->getScene(),SLOT(readFromImage(QImage)));

    ui->stackedWidget->setCurrentIndex(0);
    api=new API("http://www.chemspider.com","");

}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::moleculeInfo(QString SMILE){
    Molecule* molecule= api->search(SMILE);
    std::vector<std::array<std::string,2> > info=molecule->getInfo();
    ui->moleculeInfoView->fillInfoTable(info);
    ui->stackedWidget->setCurrentIndex(1);
    delete molecule;
}
