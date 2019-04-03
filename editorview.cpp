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

    scene->setSceneRect(0,0,window()->width(),window()->height());
    connect(ui->buttonAdd,SIGNAL(toggled(bool)),scene,SLOT(setMyModeAdd(bool)) );
    connect(ui->buttonRemove,SIGNAL(toggled(bool)),scene,SLOT(setMyModeRemove(bool)) );
    connect(ui->buttonEdit,SIGNAL(toggled(bool)),scene,SLOT(setMyModeEdit(bool)) );
    connect(ui->buttonSMILE,SIGNAL(pressed()),this,SLOT(getSMILE()));
    connect(ui->comboBox,SIGNAL(currentIndexChanged(QString)),scene,SLOT(setElement(QString)));
    connect(ui->comboBox_2,SIGNAL(currentIndexChanged(int)),scene,SLOT(setBondOrder(int)));
    connect(ui->ButtonGetInfo,SIGNAL(clicked(bool)),this,SLOT(moleculeInfo()));
    connect(ui->GenerateFromSMILE,SIGNAL(clicked(bool)),this,SLOT(fromSMILE()));
    connect(ui->buttonElements,SIGNAL(clicked(bool)),this,SLOT(elements()));
    connect(ui->buttonImage,SIGNAL(clicked(bool)),scene,SLOT(readFromImage()));
    connect(ui->buttonCamera,SIGNAL(clicked(bool)),this,SLOT(camera()));

    connect(ui->zoomSlider,SIGNAL(sliderMoved(int)),this,SLOT(zoomScene(int)));
    connect(ui->zoomSlider,SIGNAL(sliderReleased()),this,SLOT(resetSliderValue()));

    ui->buttonAdd->setIcon(QIcon(":/icons/plus.png"));
    ui->buttonRemove->setIcon(QIcon(":/icons/minus.png"));
    ui->buttonEdit->setIcon(QIcon(":/icons/moveArrow.png"));
}

EditorView::~EditorView()
{
    delete ui;
}

void EditorView::getSMILE(){
    std::vector<Atom*> atoms=scene->getAtomVector();
    if(atoms.size()==0)
        return;
    Identificator ident=Identificator() ;
    std::vector< std::vector<int> >bonds=scene->getConnectivityMatrix();
    std::string smile = ident.ind(atoms,bonds);
    ui->lineEditeSMILE->setText(QString::fromStdString(smile) );
}
void EditorView::moleculeInfo(){
    QString SMILE=ui->lineEditeSMILE->text();
    emit molInf(SMILE);
}

void EditorView::fromSMILE(){
    QString smile=ui->lineEditeSMILE->text();
    scene->drawSMILE(smile);
}
EditorScene* EditorView::getScene(){
    return scene;
}
void EditorView::elements(){
    emit elements(2);
}
void EditorView::camera(){
    emit camera(3);
}

void EditorView::setElement(QString element){
    //ui->comboBox->addItem(element);
    ui->comboBox->setCurrentText(element);
    if(ui->comboBox->currentText()!=element){
        ui->comboBox->addItem(element);
        ui->comboBox->setCurrentText(element);
    }
    scene->setElement(element);
}

void EditorView::zoomScene(int zoom){
    qreal scale = zoom/lastSliderValue;
    lastSliderValue = zoom;
    ui->graphicsView->scale(scale,scale);
}

void EditorView::resetSliderValue(){
    lastSliderValue=sliderDefault;
    ui->zoomSlider->setValue(sliderDefault);
}
