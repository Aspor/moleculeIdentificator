#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->graphicsView,SIGNAL(molInf(QString)),this,SLOT(moleculeInfo(QString)));
    connect(ui->moleculeInfoView,SIGNAL(toEditor(int)),ui->stackedWidget,SLOT(setCurrentIndex(int)));
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
