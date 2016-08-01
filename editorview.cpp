#include "editorview.h"
#include "ui_editorview.h"
#include "identificator.h"

EditorView::EditorView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EditorView)
{
    ui->setupUi(this);
    scene= new EditorScene();
    ui->graphicsView->setScene(scene);
    ui->graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    scene->setSceneRect(0,0,5000,5000);
    connect(ui->buttonAdd,SIGNAL(toggled(bool)),scene,SLOT(setMyModeAdd(bool)) );
    connect(ui->buttonRemove,SIGNAL(toggled(bool)),scene,SLOT(setMyModeRemove(bool)) );
    connect(ui->buttonEdit,SIGNAL(toggled(bool)),scene,SLOT(setMyModeEdit(bool)) );
    connect(ui->buttonSMILE,SIGNAL(pressed()),this,SLOT(getSMILE()));
    connect(ui->comboBox,SIGNAL(currentIndexChanged(QString)),scene,SLOT(setElement(QString)));
    connect(ui->comboBox_2,SIGNAL(currentIndexChanged(int)),scene,SLOT(setBondOrder(int)));
}

EditorView::~EditorView()
{
    delete ui;
}

void EditorView::getSMILE(){
    Identificator ident=Identificator() ;
    std::vector<Atom*> atoms=scene->getAtomVector();
    std::vector< std::vector<int> >bonds=scene->getConnectivityMatrix();
    std::string smile = ident.ind(atoms,bonds);
    ui->lineEditeSMILE->setText(QString::fromStdString(smile) );
}
