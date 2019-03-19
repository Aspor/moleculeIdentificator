#ifndef IMAGECAPTURE_H
#define IMAGECAPTURE_H

#include <QObject>
#include <QWidget>

#include<QtMultimedia/QCamera>
#include<QtMultimedia/QCameraInfo>
#include<QtMultimediaWidgets/QCameraViewfinder>
#include<QtMultimedia/QCameraImageCapture>
#include<QQuickWidget>
#include<QQuickItem>
#include<QQuickImageProvider>

namespace Ui {
class imageCapture;
}


class ImageCapture : public QWidget
{
    Q_OBJECT
public:
    explicit ImageCapture(QWidget *parent = 0);
signals:
    void image(QImage img);
    void back(int);

public slots:
    void start();
    void cancel();

private:
    bool checkCameraAvailability();
    QQuickWidget* qmlCamera;
    QCamera* camera;
    QCameraViewfinder* viewfinder;
    QCameraImageCapture* a;
    Ui::imageCapture *ui;
    QQuickImageProvider* imgProv;
    void initialaizeCamera();
    bool initialized;
    QQuickItem* qmlUi;

private slots:
    void getImage(QString);
};

#endif // IMAGECAPTURE_H
