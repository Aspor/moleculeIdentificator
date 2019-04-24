#ifndef EDITORSCENE_H
#define EDITORSCENE_H

#include <QGraphicsScene>
#include <QtCore>
#include <QFileDialog>
#include "atomgraphicitem.h"
#include "bondgraphicsitem.h"
#include "imagecapture.h"
#include "imagecropdialog.h"

class EditorScene: public QGraphicsScene
{
    Q_OBJECT
public:
    EditorScene();
    ~EditorScene();
    QVector<AtomGraphicItem*> getAtoms();
    QVector<BondGraphicsItem*> getBonds();
    std::vector<std::vector<int> > getConnectivityMatrix();
    std::vector<Atom *> getAtomVector();

    void addAtom(QPointF pos);
    void addAtom(AtomGraphicItem* atom);

    void addBond(AtomGraphicItem *atom1, AtomGraphicItem *atom2);
    void addBond(AtomGraphicItem *atom1, AtomGraphicItem *atom2, int bondOrder);
    void removeAtom(AtomGraphicItem *atom);
    void removeBond(BondGraphicsItem *bond);
    void editBond();
    static bool compareAtoms(AtomGraphicItem* a1, AtomGraphicItem* a2);
    void  clearOrphanedAtoms();
    int getMode();
    enum Mode {add,edit,remove};

public slots:
    void setMyModeAdd(bool toggled);
    void setMyModeEdit(bool toggled);
    void setMyModeRemove(bool toggled);
    void setElement(QString element);
    void setBondOrder(int);
    void drawSMILE(QString);

    void readFromImage(QImage img=QImage(0,0,QImage::Format_RGB32));

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent) Q_DECL_OVERRIDE;
//    void mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent) Q_DECL_OVERRIDE;

private:
    AtomGraphicItem* lastAtom;
    AtomGraphicItem* atomAt(QPointF pos);
    QString atomicSymbol;
    int bondOrder;
    int charge;
    int atomID;
    int myMode;

    QVector<BondGraphicsItem*>bonds;
    QVector<AtomGraphicItem*> atoms;

    void clean();
    void removeAll();

};

#endif // EDITORSCENE_H
