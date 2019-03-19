#ifndef IMAGECROPDIALOG_H
#define IMAGECROPDIALOG_H

#include <QWidget>
#include <QMouseEvent>
#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QImage>
#include <cropscene.h>

namespace Ui {
class ImageCropDialog;
}

class ImageCropDialog : public QWidget
{
    Q_OBJECT

public:
    explicit ImageCropDialog(QWidget *parent = nullptr);
    ~ImageCropDialog();
    void paint();
signals:
    void croppedImage(QImage img);
    void back(int);

public slots:
    void setImage(QImage img);
    void crop();
    void cancel();

private:
    Ui::ImageCropDialog *ui;
    bool scaled;
    CropScene* scene;

    QImage image;
};

#endif // IMAGECROPDIALOG_H
