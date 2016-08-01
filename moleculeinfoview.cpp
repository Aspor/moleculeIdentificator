#include "moleculeinfoview.h"
#include "ui_moleculeinfoview.h"

MoleculeInfoView::MoleculeInfoView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MoleculeInfoView)
{
    ui->setupUi(this);
}

MoleculeInfoView::~MoleculeInfoView()
{
    delete ui;
}
