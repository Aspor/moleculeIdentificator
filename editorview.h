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

signals:
    void molInf(QString);

public slots:
    void getSMILE();

private slots:
    void moleculeInfo();

private:
    Ui::EditorView *ui;

    EditorScene* scene;
};

#endif // EDITORVIEW_H
