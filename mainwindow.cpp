#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QThread"


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
    connect(ui->graphicsView,SIGNAL(camera(int)),ui->imageCapture,SLOT(start()));
    connect(ui->imageCapture,SIGNAL(back(int)),ui->stackedWidget,SLOT(setCurrentIndex(int)));
  //  connect(ui->graphicsView,SIGNAL(camera(int)),ui->imageCapture,SLOT(img()));
    connect(ui->imageCapture,SIGNAL(image(QImage)),ui->imageCropDialog,SLOT(setImage(QImage)));
    connect(ui->imageCropDialog,SIGNAL(croppedImage(QImage)),ui->graphicsView->getScene(),SLOT(readFromImage(QImage)));
    connect(ui->imageCropDialog,SIGNAL(back(int)),ui->stackedWidget,SLOT(setCurrentIndex(int)));

    api=new API("http://www.chemspider.com","");
    ui->stackedWidget->setCurrentIndex(0);
}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::moleculeInfo(QString SMILE){
    Molecule* molecule= api->search(SMILE);
    std::vector<std::pair<std::string,std::string> > info=molecule->getInfo();
    ui->moleculeInfoView->fillInfoTable(info);
    ui->stackedWidget->setCurrentIndex(1);
    delete molecule;
}
