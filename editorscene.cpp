#include "editorscene.h"
#include<QGraphicsSceneMouseEvent>
#include<QDebug>
#include<algorithm>
#include<array>
#include<imagereader.h>
#include<QImageWriter>

EditorScene::EditorScene(): QGraphicsScene()
{
    qsrand(QTime::currentTime().msec());
    srand(QTime::currentTime().msec());

    atomicSymbol='C';
    bondOrder=1;
    charge=0;
    myMode=add;
    lastAtom=nullptr;
    bonds=QVector<BondGraphicsItem*>();
    atoms=QVector<AtomGraphicItem*>();
}
EditorScene::~EditorScene(){

}

void EditorScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent){
    //ignore right and middle clicks
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
    else QGraphicsScene::mouseReleaseEvent(mouseEvent);
}

void EditorScene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent){
    if(mouseEvent->button()!=Qt::LeftButton){
        return;
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
    {
        QGraphicsItem* selectedItem=itemAt(mouseEvent->scenePos(),QTransform());
        if ( selectedItem!=nullptr){
//            QGraphicsItem* selectedItem=selectedItems()[0];
            if (selectedItem->type()==AtomGraphicItem::Type)
                removeAtom(dynamic_cast<AtomGraphicItem*>(selectedItem));
            else if(selectedItem->type()==BondGraphicsItem::Type)
                removeBond(dynamic_cast<BondGraphicsItem*> (selectedItem));
        }
        break;
    }
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

void EditorScene::addAtom(AtomGraphicItem* a){
    addItem(a);
    atoms.append(a);
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
bool EditorScene::compareAtoms(AtomGraphicItem* a1, AtomGraphicItem* a2){
    return a1->getBonds().size() < a2->getBonds().size();
}

//contruct connectivity matrix
std::vector<std::vector<int> > EditorScene::getConnectivityMatrix(){

    //Sort atoms by number of bonds so first atom in SMILE shourd have only one bond
//    std::sort(atoms.begin(),atoms.end(), compareAtoms );
//    // std::reverse(atoms.begin(),atoms.end() );
//    while(atoms.first()->getBonds().size()==0){
//        removeAtom(atoms.first());
//    }
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

//Draw molecule according to inputed SMILE
void EditorScene::drawSMILE(QString smile){
    QMap<int, AtomGraphicItem*> cyclePoints=QMap<int, AtomGraphicItem*>();
    QVector<AtomGraphicItem*> branchPoints=QVector<AtomGraphicItem*>();
    QVector<AtomGraphicItem*> atomVec=QVector<AtomGraphicItem*>();
    QVector<std::array<int,2> > cycles;
    int atomID=1;
    AtomGraphicItem* prevAtom=nullptr;
    int branchInd=-22;
    removeAll();

    for (int i=0;i<smile.size();i++){

        if(smile[i]==')'){
            branchInd--;
            prevAtom = branchPoints.takeLast();
            i++;
        }
        if (smile[i]=='('){
            branchInd++;
            branchPoints.push_back(atomVec.last());
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
        AtomGraphicItem* atom=new AtomGraphicItem( QPointF(150+i*22,150+qrand()*12.0/RAND_MAX-25*branchInd), atomSymbol,charge,atomID);
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

void EditorScene::clean(){
    //  std::sort(atoms.begin(),atoms.end(),compareAtoms);
    //  for(int i=0;i<atoms.size();i++){
    QVector<BondGraphicsItem*> b=bonds;
    qreal ang=30;
    for(int k=0;k<bonds.length();k++){
        QLineF nLine=bonds[k]->line();
        ang=pow(-1,k)*27;
        qDebug()<<ang;
        if(k>0){
            qDebug()<<"a1"<<nLine.angleTo(b[k-1]->line());
            // if(nLine.angleTo(b[k-1]->line())<25 || nLine.angleTo(b[k-1]->line())>335)
            qDebug()<<"a2"<<nLine.angleTo(b[k-1]->line());
        }
        nLine.setLength(50);
        qDebug()<< b[k]->getAtoms()[0]->getId()<<b[k]->getAtoms()[1]->getId() <<"ID";
        if( b[k]->getAtoms()[0]->getId()<b[k]->getAtoms()[1]->getId() ){
            nLine.setAngle(ang );
            b[k]->getAtoms()[0]->setPos(nLine.p2());
            b[k]->getAtoms()[1]->setPos(nLine.p1());
        }
        else{
            nLine.setAngle(ang+180 );

            b[k]->getAtoms()[1]->setPos(nLine.p2());
            b[k]->getAtoms()[0]->setPos(nLine.p1());

        }
        b[k]->setLine(nLine);
    }
    foreach (AtomGraphicItem* a, atoms) {
        a->moveBy(1,1);
    }
}
//Remove every atom from scene
void EditorScene::removeAll(){
    while(!atoms.empty()){
        AtomGraphicItem* toDel= atoms.last();
        removeAtom(toDel);
    }
}

void EditorScene::readFromImage(QImage image){// std::string file){
    removeAll();
    cv::Mat byteImage;
    //if there is no image open file dialog
    if(image.width()==0 ){
        QString file =QFileDialog::getOpenFileName();
        image.load(file);
    }
    if(image.width()==0)
        return;

    qreal scale = std::min(height()/ image.height(),width()/image.width());
    image=image.convertToFormat(QImage::Format_Grayscale8);


    QString out ="cropped"+QString::number(rand())+".jpg";

    QString path = QStandardPaths::writableLocation(QStandardPaths::PicturesLocation);


    QImage img = image.scaled(int(image.width()*scale),image.height()*scale);
    img.save(path+"/"+out);
    byteImage = cv::Mat(img.height(), img.width(), CV_8U, img.bits());
    ImageReader imgRead = ImageReader(this);
    imgRead.read(byteImage,&atoms,&bonds);
//    clearOrphanedAtoms();
}

AtomGraphicItem* EditorScene::atomAt(QPointF pos){
    return dynamic_cast <AtomGraphicItem*> (itemAt(pos,QTransform()));
}


void EditorScene::clearOrphanedAtoms(){
    std::sort(atoms.begin(),atoms.end(), compareAtoms );
    while(atoms.size() && (atoms.first()->getBonds().size()==0)){
        removeAtom(atoms.first());
    }
}

