#ifndef EDITORVIEW_H
#define EDITORVIEW_H

#include <QGraphicsView>
#include <QEvent>
#include <QTouchEvent>


class editorView : public QGraphicsView
{
    Q_OBJECT
public:
    editorView(QWidget* parent);
    bool viewportEvent(QEvent* event);
public slots:
    void setModeEdit(bool);
private:
    qreal totalScaleFactor=1;
    qreal totalDx=0;
    qreal totalDy=0;
    bool EditMode=false;
};

#endif // EDITORVIEW_H
