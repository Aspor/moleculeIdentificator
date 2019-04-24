#include "editorview.h"
#include <QDebug>

editorView::editorView(QWidget* parent) :QGraphicsView (parent)
{
    viewport()->setAttribute(Qt::WA_AcceptTouchEvents);
}

bool editorView::viewportEvent(QEvent* event){
    if(!EditMode)
        return QGraphicsView::viewportEvent(event);

    switch (event->type()) {
       case QEvent::TouchBegin:
       case QEvent::TouchUpdate:
       case QEvent::TouchEnd:
       {
           QTouchEvent *touchEvent = static_cast<QTouchEvent *>(event);
           QList<QTouchEvent::TouchPoint> touchPoints = touchEvent->touchPoints();
           if (touchPoints.count() == 2) {
               // determine scale factor
               const QTouchEvent::TouchPoint &touchPoint0 = touchPoints.first();
               const QTouchEvent::TouchPoint &touchPoint1 = touchPoints.last();
               qreal currentScaleFactor =
                       QLineF(touchPoint0.pos(), touchPoint1.pos()).length()
                       / QLineF(touchPoint0.startPos(), touchPoint1.startPos()).length();
               if (touchEvent->touchPointStates() & Qt::TouchPointReleased) {
                   // if one of the fingers is released, remember the current scale
                   // factor so that adding another finger later will continue zooming
                   // by adding new scale factor to the existing remembered value.
                   totalScaleFactor *= currentScaleFactor;
                   currentScaleFactor = 1;
               }
               setTransform(QTransform().scale(totalScaleFactor * currentScaleFactor,
                                               totalScaleFactor * currentScaleFactor));
               event->accept();
               return true;
           }
           if (touchPoints.count() == 3) {

               const QTouchEvent::TouchPoint &touchPoint0 = touchPoints.first();
               qreal dx = (touchPoint0.startPos().x()-touchPoint0.pos().x())/totalScaleFactor;
               qreal dy = (touchPoint0.startPos().y()-touchPoint0.pos().y())/totalScaleFactor;
               scene()->setSceneRect(totalDx+dx,totalDy+dy,scene()->width(),scene()->width());
               if (touchEvent->touchPointStates() & Qt::TouchPointReleased) {
                   // if one of the fingers is released, remember the current scale
                   // factor so that adding another finger later will continue zooming
                   // by adding new scale factor to the existing remembered value.
                   totalDx+=dx;
                   totalDy+=dy;
               }

               event->accept();
               return true;

           }
           break;
       }
       default:
           break;
       }
       return QGraphicsView::viewportEvent(event);

}

void editorView::setModeEdit(bool toggled){
    EditMode=toggled;
}
