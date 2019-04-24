#include "imagecapture.h"
#include <QThread>
#include"ui_imagecapture.h"


ImageCapture::ImageCapture(QWidget *parent) : QWidget(parent), ui(new Ui::imageCapture)
{
    initialized=false;
}
void ImageCapture::initialaizeCamera(){
    qmlCamera = new QQuickWidget;
    qmlCamera->setSource(QUrl("qrc:///Camera.qml"));
    QQmlEngine* engine=qmlCamera->engine();
    QQmlImageProviderBase* imgProvBase = engine->imageProvider("camera");
    ui->setupUi(this);
    QGridLayout* lay =new QGridLayout(ui->widget);
    lay->addWidget(qmlCamera);
    qmlUi =qmlCamera->rootObject();
    QObject* camera1 = qmlUi->findChild<QObject*>("camera1");
    camera = qvariant_cast<QCamera*>(camera1->property("mediaObject"));

    imgProv=static_cast<QQuickImageProvider*> (imgProvBase);
    a = new QCameraImageCapture(camera);
   // connect(a,SIGNAL(imageCaptured(int,QImage)),this,SLOT(getImage(int,QImage)));
    connect(qmlUi,SIGNAL(accept(QString)),this,SLOT(getImage(QString)));
    connect(qmlUi,SIGNAL(cancel()),this,SLOT(cancel()));
    initialized=true;
    camera->start();
}

void ImageCapture::getImage(QString imgUrl)
{
    QImage img(imgUrl);
    emit image(img);
    camera->stop();
    emit back(4);
    return;
}


void ImageCapture::start(){
    initialized? camera->start():initialaizeCamera();
}
void ImageCapture::cancel(){
    camera->stop();
    emit back(0);
}

bool checkCameraAvailability()
{
    if (QCameraInfo::availableCameras().count() > 0)
        return true;
    else
        return false;
}
