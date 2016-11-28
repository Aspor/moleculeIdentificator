#ifndef ATOMGRAPHICITEM_H
#define ATOMGRAPHICITEM_H

#include <QGraphicsItem>
#include <QtCore>
#include "atom.h"


class BondGraphicsItem;

class AtomGraphicItem: public QGraphicsTextItem, public Atom
{
    Q_OBJECT
public:
    AtomGraphicItem(QPointF pos, QString symbol,int charge, int id);
    AtomGraphicItem(QPointF pos, std::string symbol,int charge, int id);

    AtomGraphicItem(QString symbol, int charge, int id);
    ~AtomGraphicItem();
    void setElement(QString symblol);
    void setCharge(int charge);
    void setText();
    void updateText();
    void addBond(BondGraphicsItem* bond);
    void removeBond(BondGraphicsItem* bond);
    QVector<BondGraphicsItem*> getBonds();
    enum { Type = UserType + 1 };
    int type() const;

    virtual QPainterPath shape() const;

signals:
    void posChange();

protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant &value) Q_DECL_OVERRIDE;


private:
    int charge;
    int id;
    QVector<BondGraphicsItem*> bonds;

};

#endif // ATOMGRAPHICITEM_H
