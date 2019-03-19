#ifndef CROPSCENE_H
#define CROPSCENE_H

#include <QGraphicsScene>
#include<QGraphicsSceneMouseEvent>
#include<QGraphicsRectItem>

class CropScene: public QGraphicsScene
{
Q_OBJECT
public:
    CropScene();
    void mousePressEvent(QGraphicsSceneMouseEvent* event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent* event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent* event);
    void setImage(QImage image);
    QImage getImage();
private:
    QPointF start;
    QPointF end;
    QRectF selection;
    QGraphicsRectItem* selectionRect;
    bool mousePressed;
    QImage image;

};

#endif // CROPSCENE_H
