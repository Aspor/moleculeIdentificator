#include "editorscene.h"
#include<QGraphicsSceneMouseEvent>
#include<QDebug>

EditorScene::EditorScene(): QGraphicsScene()
{
    atomicSymbol='C';
    bondOrder=1;
    charge=0;
    myMode=add;
    lastAtom=nullptr;
    bonds=QVector<BondGraphicsItem*>();
    atoms=QVector<AtomGraphicItem*>();
    //   setSceneRect(0,0,0,0);

}
EditorScene::~EditorScene(){

}

void EditorScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent){
    if(mouseEvent->button()!=Qt::LeftButton){
        return;
    }
    if(myMode==add){
        if(lastAtom!=nullptr){
            QGraphicsItem* selected=itemAt(mouseEvent->scenePos(),QTransform());
            //qDebug()<<selected->pos();
            if(selected!=0 && selected->type()==lastAtom->type()){
                addBond(dynamic_cast<AtomGraphicItem*>(selected),lastAtom);
            }
            //else
            //addAtom(mouseEvent->scenePos());
        }
    }
}

void EditorScene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent){
    if(mouseEvent->button()!=Qt::LeftButton){
    }

    switch (myMode){
    case add:
        if( itemAt(mouseEvent->scenePos(),QTransform() )){
            QGraphicsItem* selected=itemAt(mouseEvent->scenePos(),QTransform());
            //qDebug()<<selected->pos();
            if(selected!=0 && selected->type()==AtomGraphicItem::Type)
                lastAtom=dynamic_cast<AtomGraphicItem*> (selected);
            else if(selected->type()==BondGraphicsItem::Type){
                dynamic_cast<BondGraphicsItem*>(selected)->nextBondOrder();
                update();

            }
        }
        else
            addAtom(mouseEvent->scenePos());
        break;

    case edit:
    {
        QGraphicsItem* selected=itemAt(mouseEvent->scenePos(),QTransform());
        if(selected!=nullptr && selected->type()==BondGraphicsItem::Type){
            dynamic_cast<BondGraphicsItem*>(selected)->nextBondOrder();
        }
        else
            QGraphicsScene::mousePressEvent(mouseEvent);
        break;
    }
    case remove:
        QGraphicsScene::mousePressEvent(mouseEvent);
        if ( selectedItems().size()){
            QGraphicsItem* selectedItem=selectedItems()[0];
            if (selectedItem->type()==AtomGraphicItem::Type)
                removeAtom(dynamic_cast<AtomGraphicItem*>(selectedItem));
            else if(selectedItem->type()==BondGraphicsItem::Type)
                removeBond(dynamic_cast<BondGraphicsItem*> (selectedItem));
            //removeItem(selectedItem);
            //delete selectedItem;
        }
        break;
    default:
        break;
    }
}


void EditorScene::addAtom(QPointF pos){
    AtomGraphicItem* atom=new AtomGraphicItem(pos,atomicSymbol,atomID,charge);
    atomID++;
    if(lastAtom!=nullptr){
        addBond(atom,lastAtom);
    }
    lastAtom=atom;
    addItem(atom);
    atoms.append(atom);
}

void EditorScene::addBond(AtomGraphicItem* atom1, AtomGraphicItem* atom2){
    if(atom1!=atom2){
        AtomGraphicItem* arr[2]={atom1,atom2};
        BondGraphicsItem* bond =new BondGraphicsItem(arr,bondOrder);
        addItem(bond);
        atom1->addBond(bond);
        atom2->addBond(bond);
        bonds.append(bond);
    }
}

void EditorScene::removeAtom(AtomGraphicItem* atom){
    while (atoms.contains(atom)){
        AtomGraphicItem* tmp = atoms.takeAt(atoms.indexOf(atom));
        while (!tmp->getBonds().empty()){
            BondGraphicsItem* tmpBond=tmp->getBonds().takeLast();
            tmp->removeBond(tmpBond);
            removeBond(tmpBond);
        }
        if(tmp==lastAtom)
            lastAtom=nullptr;
        removeItem(tmp);
        delete tmp;
    }
}

void EditorScene::removeBond(BondGraphicsItem* bond){
    while(bonds.contains(bond)){
        BondGraphicsItem* tmp =bonds.takeAt(bonds.indexOf(bond));
        removeItem(tmp);
        delete tmp;
    }

}

QVector<BondGraphicsItem *> EditorScene::getBonds(){
    return bonds;
}
QVector<AtomGraphicItem*> EditorScene::getAtoms(){
    return atoms;
}
std::vector<std::vector<int> > EditorScene::getConnectivityMatrix(){
    std::vector<std::vector<int> > conMatrix=std::vector<std::vector<int> >(atoms.size(), std::vector<int> (atoms.size(),0));
    foreach (BondGraphicsItem* bond, bonds){
        if(bond->getAtoms()[0]->getElement()=="H" || bond->getAtoms()[1]->getElement()=="H")
            continue;
        int i=atoms.indexOf(bond->getAtoms()[0]);
        int j=atoms.indexOf(bond->getAtoms()[1]);
        conMatrix[i][j]=bond->getBondOrder();
        conMatrix[j][i]=bond->getBondOrder();
        qDebug()<<"Bond i j"<<i<<j;
    }
    return conMatrix;
}
std::vector<Atom *> EditorScene::getAtomVector(){
    std::vector<Atom*> tmpList=std::vector<Atom*>();
    foreach(AtomGraphicItem* GAtom, atoms){
        Atom* atom= GAtom;
        tmpList.push_back(atom);
    }

    return tmpList;
    //return atoms.toStdList();
}
//void EditorScene::drawBrach(QString branch){

//}

//void EditorScene::drawSMILE(QString smile){
//    QVector<QString> branches=QVector<QString>(0,"");
//    QVector<int> branchPoints=QVector<int>();
//    QVector<Atom*> atomVec=QVector<Atom*>();
//    for (int i=0;i<smile.size();i++){
//        QString atomStr;
//        if(smile[i].isUpper()){
//            while (smile[i+1].isLower()){
//                atomStr.append(smile[i]);
//                i++;
//            }
//            QString atomSymbol="";
//                    atomSymbol.append(atomStr[0]);
//            int charge=0;
//            int j=1;
//            if(atomStr[j].isLetter()){
//                atomSymbol+=atomStr[j];
//                j+=1;
//            }
//            if(atomStr[j].isDigit()){
//                charge= int(atomStr[j]);
//                j++;
//            }
//            switch (toascii( (atomStr[j]))) {
//            case "=":
//                bondOrder=2;
//                break;
//            case "#":
//                bondOrder=3;
//                break;
//            case "$":
//                bondOrder=4;
//                break;
//            case ".":
//                bondOrder=0;
//            default:
//                bondOrder=1;
//                break;
//            }
//            atomVec.append(new Atom(atomSymbol,charge));
//            if(atomVec.size()>1)
//                bonds.append(new BondGraphicsItem({atomVec[atomVec.size()-1],atomVec.last()}));
//        }
//    }
//}

void EditorScene::setMyModeAdd(bool toggled){
    if(toggled)
        myMode=add;
}
void EditorScene::setMyModeEdit(bool toggled){
    if(toggled)
        myMode=edit;
}
void EditorScene::setMyModeRemove(bool toggled){
    if(toggled)
        myMode=remove;
}
void EditorScene::setElement(QString element){
    atomicSymbol=element;
}
void EditorScene::setBondOrder(int bo){
    bondOrder=bo;
}

