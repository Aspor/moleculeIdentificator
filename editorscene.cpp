#include "editorscene.h"
#include<QGraphicsSceneMouseEvent>
#include<QDebug>

EditorScene::EditorScene(): QGraphicsScene()
{
    qsrand(12344);
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
    AtomGraphicItem* atom=new AtomGraphicItem(pos,atomicSymbol,charge,atomID);
    atomID++;
    if(lastAtom!=nullptr){
        addBond(lastAtom,atom);
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

void EditorScene::drawSMILE(QString smile){
    QMap<int, AtomGraphicItem*> cyclePoints=QMap<int, AtomGraphicItem*>();
    QVector<AtomGraphicItem*> branchPoints=QVector<AtomGraphicItem*>();
    QVector<AtomGraphicItem*> atomVec=QVector<AtomGraphicItem*>();
    QVector<std::array<int,2> > cycles;
    int atomID=1;
    AtomGraphicItem* prevAtom=nullptr;
    while(!atoms.empty()){
        AtomGraphicItem* toDel= atoms.last();
        removeAtom(toDel);

    }

    for (int i=0;i<smile.size();i++){
        if (smile[i]=='('){
            branchPoints.push_back(atomVec.last());
            i++;
        }
        if(smile[i]==')'){
            prevAtom = branchPoints.takeLast();
            i++;
        }

        switch ( int(smile[i].toLatin1())) {
        case '=':
            bondOrder=2;
            i++;
            break;
        case '#':
            bondOrder=3;
            i++;
            break;
        case '$':
            bondOrder=4;
            i++;
            break;
        case '.':
            bondOrder=0;
            i++;
            break;
        default:
            bondOrder=1;
            break;
        }

        bool charged=false;
        if(smile[i]=='['){
            charged=true;
            i++;
        }
        QString atomStr;

        atomStr.append(smile[i]);
        //i++;
        if (smile[i+1].isLower()){
            i++;
            atomStr.append(smile[i]);
        }
        QString atomSymbol="";
        atomSymbol.append(atomStr);
        int charge=0;

        if(charged){
            bool negative=false;
            if(smile[i+1]=='-'){
                negative=true;
                i++;
            }
            if(smile[i+1]=='+')
                i++;

            if(smile[i+1].isDigit()){
                i++;
                charge=smile[i].toLatin1()-'0';
                if(negative)
                    charge=-charge;

                i++;
                qDebug()<<"CHARGEDIGIT"<<charge;
        }
        }
        int cycle=-1;
        if(smile[i+1].isDigit()){
            cycle=smile[i+1].toLatin1()-'0';
            qDebug()<<"CYCLE";
            i++;
        }
        AtomGraphicItem* atom=new AtomGraphicItem( QPointF(150,150), atomSymbol,charge,atomID);
        addItem(atom);
        atomVec.append(atom);
        if(cyclePoints.find(cycle)!=cyclePoints.end()){
            AtomGraphicItem* arr[2]={atom,cyclePoints[cycle]};
            bonds.append(new BondGraphicsItem(arr,bondOrder));
            addBond(atom,cyclePoints[cycle]);
        }
        else if(cycle!=-1){
            cyclePoints.insert(cycle,atom);
        }

        atomID++;
        if(atomVec.size()>1){
            addBond(atom,prevAtom);
        }
        prevAtom=atom;
    }
    atoms=atomVec;
    clean();
}

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

void EditorScene::clean(){
    foreach (AtomGraphicItem* atom,atoms) {
        QList<QGraphicsItem*> list=atom->collidingItems();
        for (int i=0;i<list.size();i++){
            if(list[i]->type()==BondGraphicsItem::Type){
                list.removeAt(i);
                i--;
            }
        }
        while(!list.empty()){
            qreal r1=(10-qrand()%20)/10.0;
            qreal r2=(10-qrand()%20)/10.0;
            atom->moveBy(r1*10,r2*10);
            list=atom->collidingItems();
            for (int i=0;i<list.size();i++){
                if(list[i]->type()==BondGraphicsItem::Type){
                    list.removeAt(i);
                    i--;
                }
            }
        }
    }
    int counter=0;
    foreach (BondGraphicsItem* bond,bonds) {
        QList<QGraphicsItem*> list=bond->collidingItems();
        for (int i=0;i<list.size();i++){
            if(list[i]->type()==AtomGraphicItem::Type){
                list.removeAt(i);
                i--;
            }
        }
        while(!list.empty()&&counter<250){
            counter++;
            qreal r1=(10-qrand()%20)/10.0;
            qreal r2=(10-qrand()%20)/10.0;
            qDebug()<<r1<<r2;
            bond->getAtoms()[1]->moveBy(r1*10.0,r2*10.0);
            r1=(20.0-qrand()%20)/10.0;
            r2=(20.0-qrand()%20)/10.0;
            bond->getAtoms()[0]->moveBy(r1*10.0,r2*10.0);
            list=bond->collidingItems();
            for (int i=0;i<list.size();i++){
                if(list[i]->type()==AtomGraphicItem::Type){
                    list.removeAt(i);
                    i--;
                }
            }
        }
    }
}
