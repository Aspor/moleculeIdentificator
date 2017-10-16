#include "editorscene.h"
#include<QGraphicsSceneMouseEvent>
#include<QDebug>
#include<algorithm>

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
    std::sort(atoms.begin(),atoms.end(), compareAtoms );
    // std::reverse(atoms.begin(),atoms.end() );
    while(atoms.first()->getBonds().size()==0){
        removeAtom(atoms.first());
    }
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
    int branchInd;
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
    //clear scene
    cv::Mat byteImage;

    //if there is no image open file dialog
    

    if(image.width()==0 ){
        QString file =QFileDialog::getOpenFileName();
         try{
        byteImage=cv::imread(file.toStdString(),cv::IMREAD_COLOR );
    }
    catch(cv::Exception ex){
        return;
    }
        image.load(file);
    }
    else
    {
        image=image.convertToFormat(QImage::Format_ARGB32);
        byteImage = cv::Mat(image.height(), image.width(), CV_8UC4, image.bits());
    }
    
    removeAll();
    cv::Mat molImg;
    //find molecule from image
    MoleculeGrabber molGrab=MoleculeGrabber();
    try
    {
    molImg=molGrab.grabMolecule(byteImage);
    }
    catch(cv::Exception ex)
    {
        return;
    }

    bondDetector bd=bondDetector();

    //bonds start and end positions
    std::vector<std::array<int, 4> >newBonds;
    int id=0;

    //atoms at bonds start and end
    AtomGraphicItem* newAtoms[2];
    newBonds= bd.detectEdges(molImg);
    newBonds=mergeLines(newBonds);
    for (int i=0;i<newBonds.size();i++){
        newAtoms[0]= new AtomGraphicItem(QPointF(newBonds[i][0],newBonds[i][1]),std::string("C") ,0,id );
        newAtoms[1]= new AtomGraphicItem(QPointF(newBonds[i][2],newBonds[i][3]),std::string("C"),0,id+1);
        id+=2;
        for(int j=0;j<2;j++){
            atoms.append(newAtoms[j]);
            addItem(newAtoms[j]);
        }
        //Check if similar bond already exist
        bool newBond=true;
        for(int k=0;k<bonds.size();k++){
                AtomGraphicItem** oldAtoms=bonds[k]->getAtoms();
                //if there is similiar bond
                if(((oldAtoms[0]->pos()-newAtoms[0]->pos()).manhattanLength()<25  && (oldAtoms[1]->pos()-newAtoms[1]->pos()).manhattanLength()<25   )||((oldAtoms[1]->pos()-newAtoms[0]->pos()).manhattanLength()<25  && (oldAtoms[0]->pos()-newAtoms[1]->pos()).manhattanLength()<25))
            {
                newBond=false;
                //bonds[k]->setBondOrder(2);
                break;
            }
        }
        //if there isn't bond between atoms bond is added.
        if(newBond){
            addBond(newAtoms[0],newAtoms[1]);
            //mergeNearBonds(bonds.last() );
        }
    }
    atomFinder atomFind;
    std::vector<AtomGraphicItem* > atomicSymbols;
    //Try to find atomic symbols from image

    atomicSymbols = atomFind.labelAtoms(molImg,std::vector<std::string>());
    //Insert new atoms to position corresponding image
    for(int i=0;i<atomicSymbols.size();i++){
        insertAtom(atomicSymbols[i]);
        //if there is an atom near added atom mergre those
        mergeNearAtoms(atomicSymbols[i],5);
    }

    //Try to connect bonds that fall short
    combBonds();

    //Merge overlapping bonds
    mergeLines(bonds);

    //move atoms to update bond positions
    for(int j=0;j<atoms.size();j++){
        atoms[j]->moveBy(0.1,0.1);
    }
    for(int j=0;j<atoms.size();j++){
        mergeNearAtoms(atoms[j],5);
    }

    //combine bonds futher appart
    for(int j=0;j<atoms.size();j++){
        atoms[j]->moveBy(0.1,0.1);
    }
    //combine bonds futher appart
    combBonds(55,250);
    for(int j=0;j<atoms.size();j++){
        mergeNearAtoms(atoms[j],5);
    }
    for(int j=0;j<atoms.size();j++){
        atoms[j]->moveBy(0.1,0.1);
    }

    qDebug()<<"REMOVE NEAR ATOMS";
    std::sort(atoms.begin(),atoms.end(), compareAtoms );
    //remove bonds that overlap atomn
    //TODO move to own function
    for(int j=0;j<atoms.size();j++){
        AtomGraphicItem* atom=atoms[j];
        qreal dist = 5;
        QList<QGraphicsItem*> nearItems = items(atom->x()-dist,atom->y()-dist,atom->boundingRect().width()+dist*2,atom->boundingRect().height()+dist*2,  Qt::IntersectsItemShape, Qt::AscendingOrder	 ,QTransform());
        for (int i=0;i<nearItems.size();i++){
            if(nearItems[i]->type()==BondGraphicsItem::Type){
                BondGraphicsItem* nearBond=dynamic_cast<BondGraphicsItem*>(nearItems[i]);
                AtomGraphicItem** bondAtoms = nearBond->getAtoms();
                if((bondAtoms[0]!=atom) &&(bondAtoms[1]!=atom )){
                    qDebug()<<"Removed atom near bond"<<j<<i;
                    //nearBond->setBondOrder(3);
                    //removeAtom(atom);
                    //i--;
                    removeBond(nearBond);
                    //j--;
                    break;
                }
            }
        }
    }
}

AtomGraphicItem* EditorScene::atomAt(QPointF pos){
    return dynamic_cast <AtomGraphicItem*> (itemAt(pos,QTransform()));
}

void EditorScene::insertAtom(AtomGraphicItem* atom){
    addItem(atom);
    atoms.push_back(atom);
}

bool EditorScene::mergeNearAtoms(AtomGraphicItem *atom,int dist){
    bool merged=false;
    //items near atom
    QList<QGraphicsItem*> nearItems = items(atom->x()-dist,atom->y()-dist,atom->boundingRect().width()+dist*2,
atom->boundingRect().height()+dist*2, Qt::IntersectsItemShape, Qt::AscendingOrder,QTransform());
    for (int i=0;i<nearItems.size();i++){
        //if item isn't atom
        if(nearItems[i]->type()!=AtomGraphicItem::Type)
            continue;

        //if item is different from current atom
        else if(nearItems[i]!=atom){
            merged=true;
            AtomGraphicItem* nearAtom=dynamic_cast<AtomGraphicItem*> (nearItems[i]);

            //if atoms have same atomic symbol merge order dosen't matter
            if(nearAtom->getElement()==atom->getElement() ){
                mergeAtoms(atom,nearAtom,true);
                nearItems = items(atom->x()-dist,atom->y()-dist,atom->boundingRect().width()+dist*2,
atom->boundingRect().height()+dist*2,Qt::IntersectsItemShape, Qt::AscendingOrder,QTransform());
                i--;
            }
            //carbon is default atom so it can be replaced
            else if(nearAtom->getElement()=="C"){
                mergeAtoms(atom,nearAtom,true );
                nearItems = items(atom->x()-dist,atom->y()-dist,atom->boundingRect().width()+dist*2,
atom->boundingRect().height()+dist*2,Qt::IntersectsItemShape, Qt::AscendingOrder, QTransform());
                i--;
            }
            else{
                mergeAtoms(nearAtom,atom,true);
                //atom gets deleted so function call should end.
                return true;
            }
        }
    }
    return merged;
}

void EditorScene::mergeAtoms(AtomGraphicItem* a,AtomGraphicItem* a2, bool ord ){
    AtomGraphicItem* atom=a;
    AtomGraphicItem* atom2=a2;
    if(!ord && a->getElement()=="C"){
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

QPointF* EditorScene::intersectingBonds(BondGraphicsItem* b1,BondGraphicsItem* b2,qreal minThresh,qreal maxThresh){
    QPointF* inter = new QPointF(-1,-1);

    //distance between bonds start and end point
    int dist[4];
    dist[0]=(b1->line().p1()-b2->line().p1()).manhattanLength();
    dist[1]=(b1->line().p2()-b2->line().p2()).manhattanLength();
    dist[2]=(b1->line().p1()-b2->line().p2()).manhattanLength();
    dist[3]=(b1->line().p2()-b2->line().p1()).manhattanLength();
    qreal minDist=INT_MAX;
    qreal maxDist=0;

    //find shortest and longest distance
    for(int i=0;i<4;i++){
        if(dist[i]<minDist)
            minDist=dist[i];
        if(dist[i]>maxDist)
            maxDist=dist[i];
    }
    //If smallest or longest distance is too long return (-1,-1)
    if(minDist>=minThresh || maxDist>maxThresh)
        return inter;

    //if lines aren't parallel return intercect point
    if(b1->line().intersect(b2->line(),inter)!=QLineF::NoIntersection){
        return inter;
    }

    //if lines are parallel return (-1,-1)
    inter->setX(-1);
    inter->setY(-1);
    return inter;
}

//Connects gap between bond endpoints if they are close enought
void EditorScene::combBonds(qreal minThresh,qreal maxThresh){
    int bondSize=bonds.size();
    for(int i=0;i<bondSize;i++){
        for(int j=i;j<bondSize;j++){
            if(!bonds[i]->hasSameAtoms(bonds[j])){
                QPointF* inter= intersectingBonds(bonds[i],bonds[j],minThresh,maxThresh);
                //if bonds intercet
                if (inter->x()!=-1){
                    AtomGraphicItem* atom = new AtomGraphicItem( *inter ,std::string("C"),0,1000);
                    AtomGraphicItem** ba_i =  bonds[i]->getAtoms();
                    AtomGraphicItem** ba_j =  bonds[j]->getAtoms();

                    //distance between intercection point and bond end points
                    qreal dist_i1= (atom->pos() - ba_i[0]->pos()).manhattanLength();
                    qreal dist_i2= ( atom->pos() - ba_i[1]->pos()).manhattanLength();

                    qreal dist_j1=(atom->pos() - ba_j[0]->pos()).manhattanLength();
                    qreal dist_j2=(atom->pos() - ba_j[1]->pos()).manhattanLength();

                    //add new bond to atoms furthest appart
                    if(dist_i1<dist_i2){
                        addBond(atom,bonds[i]->getAtoms()[1] );
                    }
                    else{
                        addBond(atom,bonds[i]->getAtoms()[0] );
                    }

                    if(dist_j1<dist_j2){
                        addBond(atom,bonds[j]->getAtoms()[1] );
                    }
                    else{
                        addBond(atom,bonds[j]->getAtoms()[0] );
                    }

                    //add atom to scene and atom container
                    atoms.append(atom);
                    addItem(atom);
                }
                delete inter;
            }
        }
    }

    for (int i=0;i<atoms.size();i++){
        mergeNearAtoms(atoms[i],2);
    } 
    //clear orphaned atoms;
    std::sort(atoms.begin(),atoms.end(), compareAtoms );
    while(atoms.first()->getBonds().size()==0){
        removeAtom(atoms.first());
    }
}

//merge lines that are close each other and almost paralell
std::vector<std::array<int, 4> > EditorScene::mergeLines(std::vector<std::array<int, 4> > lines){
    for (int i=0;i<lines.size();i++){
        QLineF l_i=QLine(lines[i][0], lines[i][1], lines[i][2], lines[i][3]);
        for (int j=i+1;j<lines.size();j++){
            QLineF l_j=QLine(lines[j][0], lines[j][1], lines[j][2], lines[j][3]);
            if( l_i.angleTo(l_j)<15 || l_j.angleTo(l_i)<15)
            {
                if(lineDistance(l_i,l_j )<5 )
                {
                    if(l_i.length()>l_j.length()){
                        QLineF tmpLine=combineLines(l_i,l_j);
                        lines[i]=std::array<int,4>{tmpLine.x1(),tmpLine.y1(),tmpLine.x2(),tmpLine.y2() };
                        lines.erase(lines.begin()+j);
                        j--;
                    }
                    else{
                        QLineF tmpLine=combineLines(l_i,l_j);
                        lines[j]=std::array<int,4>{tmpLine.x1(),tmpLine.y1(),tmpLine.x2(),tmpLine.y2() };
                        lines.erase(lines.begin()+i);
                        i--;
                        l_i=QLine(lines[i][0], lines[i][1], lines[i][2], lines[i][3]);
                    }
                }
            }
        }
    }
    return lines;
}

QVector <BondGraphicsItem*> EditorScene::mergeLines(QVector<BondGraphicsItem*> bonds){
    for (int i=0;i<bonds.size()-1;i++){
        QLineF l_i=bonds[i]->line();
        for (int j=i+1;j<bonds.size();j++){
            QLineF l_j=bonds[j]->line();
            if( l_i.angleTo(l_j)<15 || l_j.angleTo(l_i)<15)
            {
                if(lineDistance(l_i,l_j )<5)
                {
                    if(l_i.length()>l_j.length())
                    {
                        bonds[i]->setLine(combineLines(l_i,l_j));
                        removeBond(bonds[j]);
                        bonds.erase(bonds.begin()+j );
                        j--;
                    }
                    else
                    {
                        bonds[j]->setLine(combineLines( l_j,l_i));
                        removeBond(bonds[i]);
                        bonds.erase(bonds.begin()+i);
                        i--;
                        if(bonds.size()<i)
                            l_i=bonds[i]->line();
                        else
                            break;
                    }
                }
            }
        }
    }
    return bonds;
}


void EditorScene::mergeNearBonds(BondGraphicsItem* bond)
{
    BondGraphicsItem* tmp;
    QLineF l_i=bond->line();
    for (int j=0;j<bonds.size();j++){
        if(bonds[j]==bond)
            continue;
        QLineF l_j=bonds[j]->line();
        if( l_i.angleTo(l_j)<15 || l_j.angleTo(l_i)<15)
        {
           if(lineDistance(l_i,l_j )<15 )
            {
                if(l_i.length()>l_j.length()){
                    bond->setLine(combineLines(l_i,l_j) );
                    tmp=bonds[j];
                    removeBond(tmp);
                    j--;
                }
                else{
                    bonds[j]->setLine(combineLines(l_i,l_j) );
                    removeBond(bond);
                    return;
                }
            }
        }
    }
    return;
}

//combines two line segments
QLineF EditorScene::combineLines(QLineF l1,QLineF l2){
    std::array<qreal,4> xArr=std::array<qreal,4>{l1.x1(),l1.x2(),l2.x1(),l2.x2()};
    std::array<qreal,4> yArr=std::array<qreal,4>{l1.y1(),l1.y2(),l2.y1(),l2.y2()};
    qreal x1 =*std::min_element(xArr.begin(),xArr.end());
    qreal x2 =*std::max_element(xArr.begin(),xArr.end());
    qreal y1 =*std::min_element(yArr.begin(),yArr.end());
    qreal y2 =*std::max_element(yArr.begin(),yArr.end());

    //create new line with maximum length
    QLineF nLine = QLineF(x1,y1,x2,y2);

    //set angle as avarange between lines
    qreal angle=(l1.angle()+l2.angle())/2.0;
    nLine.setAngle(angle);

    //check direction of new line and reverse it if necessary
    if( (l1.angleTo(nLine) <340 && l1.angleTo(nLine) > 20) || (l1.angleTo(nLine) <340 && l1.angleTo(nLine) > 20)){
        nLine.setAngle(angle+180);
    }
    return nLine;
}

//finds minumum distance between two line segments
qreal EditorScene::lineDistance(QLineF l1, QLineF l2){
    QPointF* inter = new QPointF;
    int interType = l1.intersect(l2,inter);
    delete inter;
    //if lines intercect distance is 0
    if(interType==QLineF::BoundedIntersection){
        return 0;
    }
    qreal minDist=INT_MAX;
    qreal dist[4];

    //if lines don't intercect distance is minumum of distances between end points and second line
    dist[0]=pointLineDist(l1,l2.p1());
    dist[1]=pointLineDist(l1,l2.p2());
    dist[2]=pointLineDist(l2,l1.p1());
    dist[3]=pointLineDist(l2,l1.p2());
    for(int i=0;i<4;i++){
        if(dist[i]<minDist){
            minDist=dist[i];
        }
    }
    return minDist;
}

//distance between line segment and point from https://stackoverflow.com/a/6853926
qreal EditorScene::pointLineDist(QLineF v, QPointF p ){
    qreal A = p.x()-v.x1();
    qreal B = p.y()-v.y1();
    qreal C = v.x2()-v.x1();
    qreal D = v.y2()-v.y1();
    qreal dot =(A*C+B*D);
    qreal lenSqr=C*C+D*D;
    qreal param=-1;
    if(lenSqr!=0)
        param=dot/lenSqr;
    qreal xx,yy;
    if (param < 0) {
       xx = v.x1();
       yy = v.y1();
     }
     else if (param > 1) {
       xx = v.x2();
       yy = v.y2();
     }
     else {
       xx = v.x1() + param * C;
       yy = v.y1() + param * D;
     }
    qreal dx=p.x()-xx;
    qreal dy=p.y()-yy;
    return sqrt(dx*dx+dy+dy);
}
