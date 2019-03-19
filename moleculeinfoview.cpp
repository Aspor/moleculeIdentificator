#include "moleculeinfoview.h"
#include "ui_moleculeinfoview.h"
#include <QDebug>
#include <array>

MoleculeInfoView::MoleculeInfoView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MoleculeInfoView)
{
    ui->setupUi(this);
    connect(ui->Return,SIGNAL(clicked(bool)),this,SLOT(toEditor()));
    ui->tableWidget->setColumnCount(1);
    ui->tableWidget->setHorizontalHeaderItem(0,new QTableWidgetItem(" "));
}

MoleculeInfoView::~MoleculeInfoView()
{
    delete ui;
}
void MoleculeInfoView::fillInfoTable(std::vector<std::pair<std::string,std::string>> info){

    ui->tableWidget->setRowCount(info.size());
    for(int i=0;i<info.size();i++){
        ui->tableWidget->setVerticalHeaderItem(i,new QTableWidgetItem(QString::fromStdString(info[i].first)));
                                // QString::fromStdString( info[0]));
        ui->tableWidget->setItem(i,0,new QTableWidgetItem(QString::fromStdString(info[i].second)));
    }
}
void MoleculeInfoView::toEditor(){
    emit toEditor(0);
}
