#include "imagecropdialog.h"
#include "ui_imagecropdialog.h"
#include <QtDebug>
#include <algorithm>

ImageCropDialog::ImageCropDialog(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ImageCropDialog)
{
    scaled = false;
    ui->setupUi(this);
    scene=new CropScene();
    ui->graphicsView->setScene(scene);
    connect(ui->buttonBox,SIGNAL(accepted()),this,SLOT(crop()));
    connect(ui->buttonBox,SIGNAL(rejected()),this,SLOT(cancel()));
}

void ImageCropDialog::setImage(QImage img){
    image=img;

    if(!scaled){
        qreal scaling = std::min(qreal(window()->width()) / img.width(),qreal(window()->height())/img.height());
        ui->graphicsView->scale(scaling,scaling);
        scaled=true;
    }
    scene->setImage(image);
    show();
    ui->graphicsView->setVisible(true);
    scene->update();
}

ImageCropDialog::~ImageCropDialog()
{
    delete ui;
}

void ImageCropDialog::crop(){
    QImage cropped = scene->getImage();
    qDebug()<<"CROP"<<cropped;
    emit (croppedImage(cropped));
    image=QImage();
    scene->clear();
    emit(back(0));
}

void ImageCropDialog::cancel(){
    image=QImage();
    scene->clear();
    emit back(3);
}
