#include "cropscene.h"
#include <QtDebug>

CropScene::CropScene(): QGraphicsScene()
{
    selection = QRectF(0,0,0,0);
//    selectionRect->setZValue(2);
//    selectionRect=new QGraphicsRectItem(0,0,0,0);
}

void CropScene::mousePressEvent(QGraphicsSceneMouseEvent *event){
    start = event->scenePos();
    selection.setTopLeft(start);
    selection.setBottomRight(start);
    selectionRect->setRect(selection);
    addItem(selectionRect);
    mousePressed=true;
}

void CropScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event){
    if(mousePressed)
    {
        end = event->scenePos();
        selection.setBottomRight(end);
        selectionRect->setRect(selection);
        update();
    }
    else {
    }
}

void CropScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event){

    end = event->scenePos();
    qreal x1 = start.x();
    qreal y1 = start.y();
    qreal x2 = end.x();
    qreal y2 = end.y();

    if(x1>x2)
        std::swap(x1,x2);
    if(y1>y2)
        std::swap(y1,y2);

    selection=QRectF(QPointF(x1,y1),QPointF(x2,y2));
    selectionRect->setRect(selection);
    mousePressed=false;
}

void CropScene::setImage(QImage img){

    image= img;
    addPixmap(QPixmap::fromImage(image));
    selectionRect = new QGraphicsRectItem(0,0,0,0);
}
QImage CropScene::getImage(){
    return image.copy(selection.toRect());

}
