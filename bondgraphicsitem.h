#ifndef BONDGRAPHICSITEM_H
#define BONDGRAPHICSITEM_H

#include <QGraphicsLineItem>
#include <QGraphicsItem>
#include <QtCore>
#include "atomgraphicitem.h"


class BondGraphicsItem : public QGraphicsLineItem
{
public:
    BondGraphicsItem(AtomGraphicItem* atoms[2], int bondOrder);
    ~BondGraphicsItem();
    enum { Type = UserType + 2 };
    int type() const;
    AtomGraphicItem **getAtoms();
    int getBondOrder();
    void setBondOrder(int bo);
    void nextBondOrder();

    QRectF boundingRect() const Q_DECL_OVERRIDE;
    QPainterPath shape() const Q_DECL_OVERRIDE;

    bool hasSameAtoms(BondGraphicsItem* bond);
    bool hasSameAtoms(AtomGraphicItem* other[2]);
    bool hasSameAtoms(AtomGraphicItem* atom1,AtomGraphicItem* atom2);


public slots:
    void updatePos();

protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);


private:
    AtomGraphicItem* atoms[2];
    int bondOrder;


};

#endif // BONDGRAPHICSITEM_H
