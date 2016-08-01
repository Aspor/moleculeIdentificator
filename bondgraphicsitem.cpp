#include "bondgraphicsitem.h"
#include <QPainter>
#include <QDebug>

BondGraphicsItem::BondGraphicsItem(AtomGraphicItem* atoms[2],int bondOrder): QGraphicsLineItem()
{
    this->atoms[0]=atoms[0];
    this->atoms[1]=atoms[1];
    updatePos();
    setFlag(QGraphicsItem::ItemIsSelectable);
    this->bondOrder=bondOrder;
    setZValue(-1);

}
BondGraphicsItem::~BondGraphicsItem(){
    atoms[0]->removeBond(this);
    atoms[1]->removeBond(this);
    return;
}

AtomGraphicItem** BondGraphicsItem::getAtoms(){
    return atoms;
}
void BondGraphicsItem::updatePos(){
    setLine(atoms[0]->x()+6,atoms[0]->y()+12,atoms[1]->x()+6,atoms[1]->y()+12);
    return;
}

void BondGraphicsItem::setBondOrder(int bo){
    bondOrder=bo;
    return;
}
int BondGraphicsItem::getBondOrder(){
    return bondOrder;
}
int BondGraphicsItem::type() const{
    return Type;
}
void BondGraphicsItem::nextBondOrder(){
    bondOrder+=1;
    if(bondOrder>4)
        bondOrder=1;
}
QRectF BondGraphicsItem::boundingRect() const{
    qreal extra = (pen().width() +20 ) / 2.0;
    //return QRectF(line().p1(), line().p2());

    return QRectF(line().p1(), QSizeF(line().p2().x() - line().p1().x(),
                                      line().p2().y() - line().p1().y()))
        .normalized()
        .adjusted(-extra, -extra, extra, extra);

}
QPainterPath BondGraphicsItem::shape() const{
    QPainterPath path =QPainterPath();
    qreal angle=line().angle();
    qreal c=cos(angle*3.14/180.0);
    qreal s=sin(angle*3.14/180.0);
    qreal i=4.0;
   // QVector<QPoint> points={(line().p1()+QPointF(s*i*5,c*i*5)),  (line().p1()-QPointF(s*i*5,c*i*5)), (line().p2()+QPointF(s*i*5,c*i*5)), (line().p2()-QPointF(s*i*5,c*i*5))};
    QPolygon pol=QPolygon();
    pol.push_back((line().p1()+QPointF(s*i*5,c*i*5)).toPoint());
    pol.push_back((line().p1()-QPointF(s*i*5,c*i*5)).toPoint());
    pol.push_back((line().p2()+QPointF(s*i*5,c*i*5)).toPoint());
    pol.push_back((line().p2()-QPointF(s*i*5,c*i*5)).toPoint());
    pol.push_back((line().p1()+QPointF(s*i*5,c*i*5)).toPoint());
    path.addPolygon(pol);
    return path;
}

void BondGraphicsItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget){
    QPen myPen=pen();
    painter->setPen(myPen);
    //painter->drawLine(line());
    QVector<QLineF> lines;
    lines.push_back(line());
    qreal angle=line().angle();
    qreal c=cos(angle*3.14/180.0);
    qreal s=sin(angle*3.14/180.0);

    for(int i=0;i<bondOrder;i++){
        //QLineF l=QLineF (line().p1()+QPointF(3*i,3*i),line().p2()+QPointF(3*i,3*i));
        QLineF l= QLineF();// ::fromPolar(line().length(),angle);
        l.setP1(line().p1()+QPointF(s*i*5,c*i*5));
        l.setLength(line().length()-5);
        l.setAngle(angle);
        painter->drawLine(l);
        qDebug()<<"DRAW i"<<i;

    }
    qDebug()<<"drawn";
}
