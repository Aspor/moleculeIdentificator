#ifndef EDITORVIEW_H
#define EDITORVIEW_H

#include <QWidget>
#include"editorscene.h"


namespace Ui {
class EditorView;
}

class EditorView : public QWidget
{
    Q_OBJECT

public:
    explicit EditorView(QWidget *parent = 0);
    ~EditorView();
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
    Ui::EditorView *ui;
    const int sliderDefault =15;
    qreal lastSliderValue = 15;
    EditorScene* scene;
};

#endif // EDITORVIEW_H
