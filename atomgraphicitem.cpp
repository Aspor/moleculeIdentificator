#include "atomgraphicitem.h"
#include "bondgraphicsitem.h"
#include <QPainter>

#include "QDebug"
AtomGraphicItem::AtomGraphicItem(QPointF pos,QString atomicSymbol,int id, int charge):
    QGraphicsTextItem(), Atom(atomicSymbol.toStdString(),charge,id)
{
    this->charge=charge;
    this->atomicSymbol=atomicSymbol;
    this->id=id;
    updateText();

    setPos(mapToScene(pos));
    setZValue(2);

    setFlag(QGraphicsItem::ItemIsMovable);
    setFlag(QGraphicsItem::ItemIsSelectable);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges);

}
AtomGraphicItem::~AtomGraphicItem(){
    while (!bonds.empty()){
        BondGraphicsItem* bond = bonds.takeLast();
        delete bond;
    }
}

void AtomGraphicItem::setElement(QString symblol){
    atomicSymbol=symblol;
    updateText();
}
void AtomGraphicItem::setCharge(int charge){
    this->charge=charge;
    updateText();
}

void AtomGraphicItem::updateText(){
    if(charge) setPlainText(atomicSymbol + charge);
    else setPlainText(atomicSymbol);
}
void AtomGraphicItem::addBond(BondGraphicsItem* bond){
    bonds.push_back(bond);
}
void AtomGraphicItem::removeBond(BondGraphicsItem* bond){
    int ind=bonds.indexOf(bond);
    if(ind!=-1)
        bonds.remove(ind);
}

QVariant AtomGraphicItem::itemChange(GraphicsItemChange change, const QVariant &value)
{
    if (change == QGraphicsItem::ItemPositionChange) {
       foreach (BondGraphicsItem* bond , bonds){
           bond->updatePos();
       }
    }

    return value;
}
int AtomGraphicItem::type() const{
    return Type;
}
QVector<BondGraphicsItem*> AtomGraphicItem::getBonds(){
    return bonds;
}
