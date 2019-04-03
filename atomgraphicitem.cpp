#include "atomgraphicitem.h"
#include "bondgraphicsitem.h"
#include <QPainter>

#include "QDebug"
AtomGraphicItem::AtomGraphicItem(QPointF pos,QString atomicSymbol,int charge, int id):
    QGraphicsTextItem(), Atom(atomicSymbol.toStdString(),charge,id)
{
    this->charge=charge;
    this->atomicSymbol = atomicSymbol.toStdString();
    this->id=id;
    updateText();
    setFont(QFont("Times",25));

    setPos(mapToScene(pos));
    setZValue(2);

    setFlag(QGraphicsItem::ItemIsMovable);
    setFlag(QGraphicsItem::ItemIsSelectable);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges);
}

AtomGraphicItem::AtomGraphicItem(QPointF pos,std::string atomicSymbol,int charge, int id):
    QGraphicsTextItem(), Atom(atomicSymbol,charge,id)
{
    this->charge=charge;
    setFont(QFont("Times",25));

    this->atomicSymbol = atomicSymbol;
    this->id=id;
    updateText();

    setPos(mapToScene(pos));
    setZValue(2);

    setFlag(QGraphicsItem::ItemIsMovable);
    setFlag(QGraphicsItem::ItemIsSelectable);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges);
}



AtomGraphicItem::AtomGraphicItem(QString atomicSymbol, int charge ,int id):
    QGraphicsTextItem(), Atom(atomicSymbol.toStdString(),charge,id)
{
    setFont(QFont("Times",25));

    this->charge=charge;
    this->atomicSymbol= atomicSymbol.toStdString();
    this->id=id;
    updateText();
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
    atomicSymbol=symblol.toStdString();
    updateText();
}
void AtomGraphicItem::setCharge(int charge){
    this->charge=charge;
    updateText();
}

void AtomGraphicItem::updateText(){
    QString numstr;
    QString atomString ="";
    atomString=atomString.fromStdString(atomicSymbol);
    numstr.setNum(charge);
    if(charge) setPlainText(atomString +  numstr);
    else setPlainText(atomString);
}
void AtomGraphicItem::addBond(BondGraphicsItem* bond){
    bonds.push_back(bond);
}
void AtomGraphicItem::removeBond(BondGraphicsItem* bond){
    int ind=bonds.indexOf(bond);
    if(ind!=-1)
        bonds.remove(ind);
}

//When atom is moved bonds are also updated
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

//QPainterPath AtomGraphicItem::shape() const{
//    QPainterPath path;
//    path.addRect(-6,-6,124,124);
//    return path;
//}
