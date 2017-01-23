#include "editorscene.h"
#include<QGraphicsSceneMouseEvent>
#include<QDebug>

EditorScene::EditorScene(): QGraphicsScene()
{
    qsrand(QTime().msec());
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

    //ignore right and middle clics
    if(mouseEvent->button()!=Qt::LeftButton){
        return;
    }


    //new bonds are added by dragging from one atom to another
    if(myMode==add){
        if(lastAtom!=nullptr){
            QGraphicsItem* selected=itemAt(mouseEvent->scenePos(),QTransform());
            if(selected!=0 && selected->type()==lastAtom->type()){
                addBond(dynamic_cast<AtomGraphicItem*>(selected),lastAtom);
            }
        }
    }
}


void EditorScene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent){
    if(mouseEvent->button()!=Qt::LeftButton){
    }


    switch (myMode){
    case add:

        //if there is something under cursor
        if( itemAt(mouseEvent->scenePos(),QTransform() )){
            QGraphicsItem* selected=itemAt(mouseEvent->scenePos(),QTransform());
            //qDebug()<<selected->pos();

            //if clicked item is atom it is selected.
            if(selected!=0 && selected->type()==AtomGraphicItem::Type){
                if(lastAtom!=selected)
                    lastAtom=dynamic_cast<AtomGraphicItem*> (selected);

               //If it was alredy selected its element is changed to chosen
                else
                    lastAtom->setElement(atomicSymbol);
            }
            //if sected item is bond increase bond order by 1
            else if(selected->type()==BondGraphicsItem::Type){
                dynamic_cast<BondGraphicsItem*>(selected)->nextBondOrder();
                update();
            }
        }

        // if there wasn't anything under cursor add a new atom
        else
            addAtom(mouseEvent->scenePos());
        break;


    case edit:
    {
        QGraphicsItem* selected=itemAt(mouseEvent->scenePos(),QTransform());

        //clikcing bonds increases bond order by one
        if(selected!=nullptr && selected->type()==BondGraphicsItem::Type){
            dynamic_cast<BondGraphicsItem*>(selected)->nextBondOrder();
            update();
        }
        else
            QGraphicsScene::mousePressEvent(mouseEvent);
        break;
    }

    // remove item under cursor
    case remove:
        QGraphicsScene::mousePressEvent(mouseEvent);
        if ( selectedItems().size()){
            QGraphicsItem* selectedItem=selectedItems()[0];
            if (selectedItem->type()==AtomGraphicItem::Type)
                removeAtom(dynamic_cast<AtomGraphicItem*>(selectedItem));
            else if(selectedItem->type()==BondGraphicsItem::Type)
                removeBond(dynamic_cast<BondGraphicsItem*> (selectedItem));
        }
        break;
    default:
        break;
    }
}

//Add atom to pos
void EditorScene::addAtom(QPointF pos){
    AtomGraphicItem* atom=new AtomGraphicItem(pos,atomicSymbol,charge,atomID);
    atomID++;

    //add bond to previosly chosen atom
    if(lastAtom!=nullptr){
        addBond(lastAtom,atom);
    }
    lastAtom=atom;
    addItem(atom);
    atoms.append(atom);
}

//add bond between atom1 and atom2
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
//add bond with bondOrder between atom1 and atom2

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

//Remove atom and every bond from it
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

//compare number of bonds between atom for sorting atoms vector
bool compareAtoms(AtomGraphicItem* a1, AtomGraphicItem* a2){
    return a1->getBonds().size() < a2->getBonds().size();
}

//contruct connectivity matrix
std::vector<std::vector<int> > EditorScene::getConnectivityMatrix(){

   //Sort atoms by number of bonds so first atom in SMILE shourd have only one bond
   // std::sort(atoms.begin(),atoms.end(), compareAtoms );

    std::vector<std::vector<int> > conMatrix=std::vector<std::vector<int> >(atoms.size(), std::vector<int> (atoms.size(),0));

    //we are only intrested in bonds
    foreach (BondGraphicsItem* bond, bonds){
        //ignore hydrogens, they are reduntant in SMILES
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

//create and return std vector of Atom items
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


//Draw molecule according to inputted SMILE
void EditorScene::drawSMILE(QString smile){
    QMap<int, AtomGraphicItem*> cyclePoints=QMap<int, AtomGraphicItem*>();
    QVector<AtomGraphicItem*> branchPoints=QVector<AtomGraphicItem*>();
    QVector<AtomGraphicItem*> atomVec=QVector<AtomGraphicItem*>();
    QVector<std::array<int,2> > cycles;
    int atomID=1;
    AtomGraphicItem* prevAtom=nullptr;

    removeAll();

    for (int i=0;i<smile.size();i++){
        if (smile[i]=='('){
            branchPoints.push_back(atomVec.last());
            i++;
        }
        if(smile[i]==')'){
            prevAtom = branchPoints.takeLast();
            i++;
        }

        //switch case for bond order
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

        //If atom has charge it's between square bracets
        bool charged=false;
        if(smile[i]=='['){
            charged=true;
            i++;
        }
        QString atomStr;

        //Atomic symbol is either one upper case letter or uppercase and lowercase letter
        atomStr.append(smile[i]);
        if (smile[i+1].isLower()){
            i++;
            atomStr.append(smile[i]);
        }
        QString atomSymbol="";
        atomSymbol.append(atomStr);
        int charge=0;

        //if atom is charged charge is after Atomic symbol
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
//TODO
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
}
//Remove every atom from scene
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
    //atoms at bonds start and end
    AtomGraphicItem* newAtoms[2];
    newBonds= bd.detectEdges(file);

    for (int i=0;i<newBonds.size();i++){
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

            //if there already was an atom at position added atom is deleted and previous atom used instead
            else{
                removeAtom(newAtoms[j]);
                newAtoms[j]= dynamic_cast<AtomGraphicItem*> ( itemAt(newAtoms[j]->pos(),QTransform()));
            }
        }

        //Check if similar bond already exist
        bool newBond=true;
        for(int k=0;k<bonds.size();k++){
            if(bonds[k]->hasSameAtoms( newAtoms[0],newAtoms[1])){
                newBond=false;
               // bonds[k]->nextBondOrder();
                break;
            }
        }
        //if there isn't bond between atoms bond is added.
        if(newBond){
            addBond(newAtoms[0],newAtoms[1]);
            //mergeNearBonds(bonds.last() );
        }
        //}
    }

    atomFinder atomFind;
    std::vector<AtomGraphicItem* > atomicSymbols;
    //Try to find atomic symbols from imafe
    atomicSymbols = atomFind.labelAtoms(file,std::vector<std::string>());

    //Insert new atoms to position corresponding image
    for(int i=0;i<atomicSymbols.size();i++){
        insertAtom(atomicSymbols[i]);
//        AtomGraphicItem* newAtom=atomicSymbols[i];
        //merge atoms that are too close each other
        mergeNearAtoms(atomicSymbols[i]);
    }
}

AtomGraphicItem* EditorScene::atomAt(QPointF pos){
    return dynamic_cast <AtomGraphicItem*> (itemAt(pos,QTransform()));
}

void EditorScene::insertAtom(AtomGraphicItem* atom){
    addItem(atom);
    atoms.push_back(atom);
}

void EditorScene::mergeNearAtoms(AtomGraphicItem *atom){

    //items near atom
    QList<QGraphicsItem*> nearItems = items(atom->x()-20,atom->y()-20,atom->boundingRect().width()+40,atom->boundingRect().height()+40,
                                            Qt::IntersectsItemShape, Qt::AscendingOrder	 ,QTransform());

    for (int i=0;i<nearItems.size();i++){

        //if item isn't atom nothing is done
        if(nearItems[i]->type()!=AtomGraphicItem::Type)
            continue;

        //if item is different from current atom
        if(nearItems[i]!=atom){
            AtomGraphicItem* nearAtom=dynamic_cast<AtomGraphicItem*> (nearItems[i]);

            //if atoms have same atomic symbol merge order dosen't matter
            if(nearAtom->getElement()==atom->getElement() ){
                mergeAtoms(atom,nearAtom);
            }
            //carbon is default atom so it can be replaced
            else if(nearAtom->getElement()=="C"){
                mergeAtoms(atom,nearAtom );
            }

            else{
                mergeAtoms(nearAtom,atom);
                return;
            }
        }
    }
    return;
}


void EditorScene::mergeAtoms(AtomGraphicItem* a,AtomGraphicItem* a2 ){
    AtomGraphicItem* atom=a;
    AtomGraphicItem* atom2=a2;

        if(a->getElement()=="C"){
            atom=a2;
            atom2=a;
        }
    //move atom to point between atom1 and atom2
    atom->moveBy( (atom2->x()-atom->x())/2,(atom2->y()-atom->y())/2 );

    //merge all bonds from atom to be deleted
    for(int i=0;i<atom2->getBonds().size();i++){
        BondGraphicsItem* mergeBond=atom2->getBonds()[i];
        AtomGraphicItem** mergeAtoms= mergeBond->getAtoms();

        //atom2 will be deleted so we replace it with atom for new bond
        if(mergeAtoms[0]==atom2 ){
            mergeAtoms[0]=atom;
        }
        else{
            mergeAtoms[1]=atom;
        }
        if(mergeAtoms[0]!=mergeAtoms[1]){
            addBond(mergeAtoms[0],mergeAtoms[1],mergeBond->getBondOrder() );
        }
    }
    removeAtom(atom2);
    return;
}


//TODO FIXME
void EditorScene::mergeNearBonds(BondGraphicsItem *bond){
    QList<QGraphicsItem*> collidingItems = bond->collidingItems();
    qDebug()<<"col"<<collidingItems.size();
    for(int i=0;i<collidingItems.size();i++){
        if(collidingItems[i]->type()!=BondGraphicsItem::Type )
            continue;
        qDebug()<<"bondBond2";
        BondGraphicsItem* bond2= dynamic_cast<BondGraphicsItem*>(collidingItems[i]);
        bond->nextBondOrder();
        return;
        if(bond->getAtoms()[0]==bond2->getAtoms()[0] ||  bond->getAtoms()[1]==bond2->getAtoms()[1] || bond->getAtoms()[0]==bond2->getAtoms()[1] ||  bond->getAtoms()[1]==bond2->getAtoms()[0] ){
            qDebug()<<"SAME";
        }
        qDebug()<<"ATOM BOND"<<bond->getAtoms()[0]<<bond->getAtoms()[1]<<bond2->getAtoms()[0]<<bond2->getAtoms()[1];
        if(mergeBonds(bond,bond2)){
            i--;
            collidingItems=bond->collidingItems();
        }
    }
}

//TODO
bool EditorScene::mergeBonds(BondGraphicsItem *bond, BondGraphicsItem *bond2){
    AtomGraphicItem** atoms1= bond->getAtoms();
    AtomGraphicItem** atom2 = bond2->getAtoms();
    //merge atoms that are near each other

    int dist[4];

    dist[0]=(atoms1[0]->pos()-atom2[0]->pos()).manhattanLength();
    dist[1]=(atoms1[0]->pos()-atom2[1]->pos()).manhattanLength();
    dist[2]=(atoms1[1]->pos()-atom2[1]->pos()).manhattanLength();
    dist[3]=(atoms1[1]->pos()-atom2[0]->pos()).manhattanLength();

    if(dist[0]<55 && atoms1[0]!=atom2[0] ){
        mergeAtoms(atoms1[0],atom2[0]);
        return true;
    }
    if(dist[1]<55 && atoms1[0]!=atom2[1] ){
        mergeAtoms(atoms1[0],atom2[1]);
        return true;
    }
    if(dist[2]<55 && atoms1[1]!=atom2[1] ){
        mergeAtoms(atoms1[1],atom2[1]);
        return true;
    }
    if(dist[3]<55 && atoms1[1]!=atom2[0] ){
        mergeAtoms(atoms1[1],atom2[0]);
        return true;
    }
    return false;
//    if((atoms1[0]->pos()-atom2[0]->pos()).manhattanLength() <
//            (atoms1[1]->pos()-atom2[1]->pos()).manhattanLength()){
//        if(atoms1[0]!=atom2[0]){
//            mergeAtoms(atoms1[0],atom2[0]);
//        }
//        if(atoms1[1]!=atom2[1] && atoms1[0]!=atom2[1] ){
//            mergeAtoms(atoms1[1],atom2[1]);
//        }
//    }
//    else{
//        if(atoms1[1]!=atom2[0]){
//            mergeAtoms(atoms1[1],atom2[0]);
//        }
//        if(atoms1[0]!=atom2[1] && atoms1[1]!=atom2[1] ){
//            mergeAtoms(atoms1[0],atom2[1]);
//        }
//    }
}
