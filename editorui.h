#ifndef EDITORUI_H
#define EDITORUI_H

#include <QWidget>
#include"editorscene.h"


namespace Ui {
class EditorUi;
}

class EditorUi : public QWidget
{
    Q_OBJECT

public:
    explicit EditorUi(QWidget *parent = 0);
    ~EditorUi();
    EditorScene* getScene();


signals:
    void molInf(QString);
    void elements(int );
    void camera(int);

public slots:
    void getSMILE();
    void setElement(QString);

private slots:
    void moleculeInfo();
    void fromSMILE();
    void elements();
    void camera();
    void zoomScene(int);
    void resetSliderValue();

private:
    Ui::EditorUi *ui;
    const int sliderDefault =15;
    qreal lastSliderValue = 15;
    EditorScene* scene;
    qreal totalScaleFactor;
};

#endif // EDITORUI_H
