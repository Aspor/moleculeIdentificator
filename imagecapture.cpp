#include "imagecapture.h"
#include <QThread>
#include"ui_imagecapture.h"

ImageCapture::ImageCapture(QWidget *parent) : QWidget(parent), ui(new Ui::imageCapture)
{
    qmlCamera = new QQuickWidget;
    qmlCamera->setSource(QUrl("qrc:///Camera.qml"));
    QQmlEngine* engine=qmlCamera->engine();
    QQmlImageProviderBase* imgProvBase = engine->imageProvider("camera");
    ui->setupUi(this);
    QGridLayout* lay =new QGridLayout(ui->widget);
    lay->addWidget(qmlCamera);
    QQuickItem* qmlUi =qmlCamera->rootObject();
    QObject* camera1 = qmlUi->findChild<QObject*>("camera1");
    camera = qvariant_cast<QCamera*>(camera1->property("mediaObject"));

    imgProv=static_cast<QQuickImageProvider*> (imgProvBase);
    qDebug()<<"img"<<imgProvBase;


    a = new QCameraImageCapture(camera);
   // connect(a,SIGNAL(imageCaptured(int,QImage)),this,SLOT(getImage(int,QImage)));
    connect(qmlUi,SIGNAL(accept(QString,QSizeF,QSizeF)),this,SLOT(getImage(QString,QSizeF,QSizeF)));
}

void ImageCapture::getImage(QString imgUrl,QSizeF s,QSizeF s2)
{
    qDebug()<<"img2"<<imgProv;

    QImage img;
    QSize ss=s.toSize();
    QSize ss2=s2.toSize();
    QString name =imgUrl.split("/").last();
    img = imgProv->requestImage(name,&ss,ss2);

   // img.load(imgUrl);
    qDebug()<<"GOT IMAGE"<<img<<imgUrl;
    emit image(img);
    //camera->stop();
    emit back(0);
    return;
}

bool checkCameraAvailability()
{
    if (QCameraInfo::availableCameras().count() > 0)
        return true;
    else
        return false;
}
