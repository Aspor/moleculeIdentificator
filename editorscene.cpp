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
            if(selected!=0 && selected->type()==AtomGraphicItem::Type){
                if(lastAtom!=selected)
                    lastAtom=dynamic_cast<AtomGraphicItem*> (selected);
                else
                    lastAtom->setElement(atomicSymbol);
            }
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
            update();
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
void EditorScene::addBond(AtomGraphicItem* atom1, AtomGraphicItem* atom2,int bondOrder){
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

bool compareAtoms(AtomGraphicItem* a1, AtomGraphicItem* a2){
    return a1->getBonds().size() < a2->getBonds().size();
}

std::vector<std::vector<int> > EditorScene::getConnectivityMatrix(){

    //Sort atoms by number of bonds so first atom in SMILE shourd have only one bond
    std::sort(atoms.begin(),atoms.end(), compareAtoms );

    std::vector<std::vector<int> > conMatrix=std::vector<std::vector<int> >(atoms.size(), std::vector<int> (atoms.size(),0));
    foreach (BondGraphicsItem* bond, bonds){
        //ignore hydrogens.
        if(bond->getAtoms()[0]->getElement()=="H" || bond->getAtoms()[1]->getElement()=="H")
            continue;
        int i=atoms.indexOf(bond->getAtoms()[0]);
        int j=atoms.indexOf(bond->getAtoms()[1]);
        conMatrix[i][j]=bond->getBondOrder();
        conMatrix[j][i]=bond->getBondOrder();
        //qDebug()<<"BOND"<<i<<j;
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
            //bonds.append(new BondGraphicsItem(arr,bondOrder));
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


//Checks for colliding atoms and randomly moves them until there aren't any
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
    //    int counter=0;
    //    foreach (BondGraphicsItem* bond,bonds) {
    //        QList<QGraphicsItem*> list=bond->collidingItems();
    //        for (int i=0;i<list.size();i++){
    //            if(list[i]->type()==AtomGraphicItem::Type){
    //                list.removeAt(i);
    //                i--;
    //            }
    //        }
    //        while(!list.empty()&&counter<250){
    //            counter++;
    //            qreal r1=(10-qrand()%20)/10.0;
    //            qreal r2=(10-qrand()%20)/10.0;
    //            qDebug()<<r1<<r2;
    //            bond->getAtoms()[1]->moveBy(r1*10.0,r2*10.0);
    //            r1=(20.0-qrand()%20)/10.0;
    //            r2=(20.0-qrand()%20)/10.0;
    //            bond->getAtoms()[0]->moveBy(r1*10.0,r2*10.0);
    //            list=bond->collidingItems();
    //            for (int i=0;i<list.size();i++){
    //                if(list[i]->type()==AtomGraphicItem::Type){
    //                    list.removeAt(i);
    //                    i--;
    //                }
    //            }
    //        }
    //    }
}
void EditorScene::removeAll(){
    while(!atoms.empty()){
        AtomGraphicItem* toDel= atoms.last();
        removeAtom(toDel);
    }
}

void EditorScene::readFromImage(){
    //clear scene
    removeAll();
    std::string file =QFileDialog::getOpenFileName().toStdString();




    bondDetector bd =bondDetector();

    //bonds start and end positions
    std::vector<std::array<int, 4> >newBonds;
    int id=0;
    //atoms at bonds start and end?
    AtomGraphicItem* newAtoms[2];

    newBonds= bd.detectEdges(file);

    for (int i=0;i<newBonds.size();i++){
        qDebug()<<"i"<<i<<newBonds.size();
        newAtoms[0]= new AtomGraphicItem(QPointF(newBonds[i][0],newBonds[i][1]),std::string("C") ,0,id );
        newAtoms[1]= new AtomGraphicItem(QPointF(newBonds[i][2],newBonds[i][3]),std::string("C"),0,id+1);
        id+=2;
        bool addedAtoms=false;

        for(int j=0;j<2;j++){
            //If there isn't Qgraphicsitem at pos or item isn't atom, add new atom at pos.
            if(itemAt(newAtoms[j]->pos(),QTransform())==nullptr ||
                    itemAt(newAtoms[j]->pos() ,QTransform())->type()!=AtomGraphicItem::Type){
                atoms.append(newAtoms[j]);
                addItem(newAtoms[j]);

                //find colliding items and remove colliding bonds
                QList<QGraphicsItem*> list=newAtoms[j]->collidingItems();
                for (int k=0;k<list.size();k++){
                    if(list[k]->type()==BondGraphicsItem::Type){
                        list.removeAt(k);
                        k--;
                    }
                }
                addedAtoms=true;

                //If new atom collides with some other atom they probably should be same atom
                if(!list.empty()){
                    qDebug()<<"List atom"<<list.size();
                    removeAtom(newAtoms[j]);
                    newAtoms[j]=dynamic_cast<AtomGraphicItem*> (list[0]);
                }
            }

            //if there already was an atom at position
            else{
                removeAtom(newAtoms[j]);
                newAtoms[j]= dynamic_cast<AtomGraphicItem*> ( itemAt(newAtoms[j]->pos(),QTransform()));
            }
        }
        //if(addedAtoms){
        //Check if similar bond already exist
        bool newBond=true;
        for(int k=0;k<bonds.size();k++){
            if(bonds[k]->hasSameAtoms( newAtoms[0],newAtoms[1])){
                newBond=false;
                //bonds[k]->nextBondOrder();
                break;
            }
        }
        if(newBond){
            addBond(newAtoms[0],newAtoms[1]);
        }
        //}
    }

    atomFinder atomFind;
    std::vector<AtomGraphicItem* > atomicSymbols;
    atomicSymbols = atomFind.labelAtoms(file,std::vector<std::string>());
    for(int i=0;i<atomicSymbols.size();i++){
        insertAtom(atomicSymbols[i]);
//        AtomGraphicItem* newAtom=atomicSymbols[i];
        mergeNearAtoms(atomicSymbols[i]);
    }



    //QVector<AtomGraphicItem*> NAtoms;
    //    std::vector<std::array<int,2> > atomPos;
    //    BlobDetector blobDect = BlobDetector();
    //    atomPos =  blobDect.getBlobPos(file);

    //    for (int i=0;i<atomPos.size();i++){
    //        AtomGraphicItem* NAtom = dynamic_cast<AtomGraphicItem*>  ( atomAt(QPointF(atomPos[i][0],atomPos[i][1])));

    //        if(NAtom !=nullptr && NAtom->type()==AtomGraphicItem::Type){
    //            NAtom->setElement("O");
    //        }
    //    }
    //    std::vector<std::array<int,2> > atomPoss;
    //    qDebug()<<"ATOMS Size"<<atoms.size();


    //    for(int i=0;i<atoms.size();i++){
    //        atomicSymbols.push_back(atoms[i]->getElement());
    //        std::array<int,2> pos =  {atoms[i]->pos().x()+2 , atoms[i]->pos().y()+2};
    //        atomPoss.push_back(pos);
    //    }



}

AtomGraphicItem* EditorScene::atomAt(QPointF pos){
    return dynamic_cast <AtomGraphicItem*> (itemAt(pos,QTransform()));
}

void EditorScene::insertAtom(AtomGraphicItem* atom){
    addItem(atom);
    atoms.push_back(atom);
}

void EditorScene::mergeNearAtoms(AtomGraphicItem *atom){
    QList<QGraphicsItem*> nearItems = items(atom->x()-20,atom->y()-20,atom->boundingRect().width()+40,atom->boundingRect().height()+40,
                                            Qt::IntersectsItemShape, Qt::AscendingOrder	 ,QTransform());
    for (int i=0;i<nearItems.size();i++){
        qDebug()<<"near"<<i;
        if(nearItems[i]->type()==AtomGraphicItem::Type){
            if(nearItems[i]!=atom){
                AtomGraphicItem* nearAtom=dynamic_cast<AtomGraphicItem*> (nearItems[i]);
                if(nearAtom->getElement()==atom->getElement() ){
                    mergeAtoms(atom,nearAtom);
                }
                else if(nearAtom->getElement()=="C"){
                    mergeAtoms(atom,nearAtom );
                }
                else{
                    qDebug()<<"atom";
                    mergeAtoms(nearAtom,atom);
                    return;
                }
            }
        }
    }
    return;
}
void EditorScene::mergeAtoms(AtomGraphicItem* atom,AtomGraphicItem* atom2 ){
    atom->moveBy( (atom2->x()-atom->x())/2,(atom2->y()-atom->y())/2 );
   // int boundSize =atom2->getBonds().size();
    for(int i=0;i<atom2->getBonds().size();i++){
        qDebug()<<"Merge"<<i;
        BondGraphicsItem* mergeBond=atom2->getBonds()[i];
        AtomGraphicItem** mergeAtoms= mergeBond->getAtoms();

        qDebug()<<mergeAtoms[0]<<mergeAtoms[1];
        if(mergeAtoms[0]==atom2 ){
            mergeAtoms[0]=atom;
        }
        else{
            mergeAtoms[1]=atom;
        }
        qDebug()<<"addBound";
        if(mergeAtoms[0]!=mergeAtoms[1])
            addBond(mergeAtoms[0],mergeAtoms[1],mergeBond->getBondOrder() );
    }

    qDebug()<<"removeAtom";
    removeAtom(atom2);
    return;
}
