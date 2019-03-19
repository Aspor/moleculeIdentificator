#include "imagereader.h"
#include <array>

#include <ImageRecognition/atomfinder.h>
#include <ImageRecognition/bonddetector.h>
#include <ImageRecognition/moleculegrabber.h>

#include<editorscene.h>

ImageReader::ImageReader(EditorScene *scene)
{
    this->scene=scene;
}
void ImageReader::read(cv::Mat byteImg, QVector<AtomGraphicItem*> *a, QVector<BondGraphicsItem*> *b  ) {
    atoms=a;
    bonds=b;
    BondDetector bondDetector = BondDetector();
    AtomFinder atomFinder=AtomFinder();
    MoleculeGrabber molGrab = MoleculeGrabber();

    //cv::Mat molImg = molGrab.grabMolecule(byteImg);
    cv::Mat molImg = byteImg;

    std::vector<std::array<double, 4> >newBonds;
    AtomGraphicItem* newAtoms[2];
    qreal scale =1;
    qDebug()<<"SCALE"<<scale<<"WIDTHS"<<scene->width() << molImg.cols<<"HEIGTHS" << scene->height()<<molImg.rows;
    int id = 0;
    newBonds = bondDetector.detectEdges(molImg);
    newBonds = mergeLines(newBonds);
    for (uint i=0;i<newBonds.size();i++){
        newAtoms[0]= new AtomGraphicItem(QPointF(newBonds[i][0]*scale,newBonds[i][1]*scale),std::string("C") ,0,id);
        newAtoms[1]= new AtomGraphicItem(QPointF(newBonds[i][2]*scale,newBonds[i][3]*scale),std::string("C"),0,id+1);
        id+=2;
        for(int j=0;j<2;j++){
            atoms->append(newAtoms[j]);
            scene->addItem(newAtoms[j]);
        }
        //Check if similar bond already exist
        bool newBond=true;
//        for(int k=0;k<bonds.size();k++){
//                AtomGraphicItem** oldAtoms=bonds[k]->getAtoms();
//                //if there is similiar bond
//                if(((oldAtoms[0]->pos()-newAtoms[0]->pos()).manhattanLength()<25  && (oldAtoms[1]->pos()-newAtoms[1]->pos()).manhattanLength()<25   )||((oldAtoms[1]->pos()-newAtoms[0]->pos()).manhattanLength()<25  && (oldAtoms[0]->pos()-newAtoms[1]->pos()).manhattanLength()<25))
//            {
//                newBond=false;
//                //bonds[k]->setBondOrder(2);
//                break;
//            }int
//        }

        //if there isn't bond between atoms bond is added.
        if(newBond){
            scene->addBond(newAtoms[0],newAtoms[1]);
            //mergeNearBonds(bonds->last() );
        }
    }
    mergeLines();
    std::vector<AtomGraphicItem* > atomicSymbols;
    //Try to find atomic symbols from image

    atomicSymbols = atomFinder.labelAtoms(molImg,std::vector<std::string>());
   // Insert new atoms to position corresponding image
    for(uint i=0;i<atomicSymbols.size();i++){
        scene->addAtom(atomicSymbols[i]);
        //if there is an atom near added atom mergre those
        //mergeNearAtoms(atomicSymbols[i],5);
    }
    scene->clearOrphanedAtoms();

    //Merge overlapping bonds
    mergeLines();
    //move atoms to update bond positions
    for(int j=0;j<atoms->size();j++){
        atoms->at(j)->moveBy(0.1,0.1);
    }
    for(int j=0;j<atoms->size();j++){
        mergeNearAtoms(atoms->at(j),2);
    }
    combBonds();
    removeAtomsNearBonds();
    qDebug()<<"REMOVE NEAR ATOMS";
}


bool ImageReader::mergeNearAtoms(AtomGraphicItem *atom,int dist){
    bool merged=false;
    //items near atom
    QList<QGraphicsItem*> nearItems = scene->items(atom->x()-dist,atom->y()-dist,atom->boundingRect().width()+dist*2,
atom->boundingRect().height()+dist*2, Qt::IntersectsItemShape, Qt::AscendingOrder,QTransform());
    for (int i=0;i<nearItems.size();i++){
        //if item isn't atom skip it
        if(nearItems[i]->type()!=AtomGraphicItem::Type)
            continue;
        //if item is different from current atom
        else if(nearItems[i]!=atom){
            merged=true;
            AtomGraphicItem* nearAtom=dynamic_cast<AtomGraphicItem*> (nearItems[i]);

            //if atoms have same atomic symbol merge order dosen't matter
            if(nearAtom->getElement()==atom->getElement() ){
                mergeAtoms(atom,nearAtom,true);
                nearItems = scene->items(atom->x()-dist,atom->y()-dist,atom->boundingRect().width()+dist*2,
atom->boundingRect().height()+dist*2,Qt::IntersectsItemShape, Qt::AscendingOrder,QTransform());
                i--;
            }
            //carbon is default atom so it can be replaced
            else if(nearAtom->getElement()=="C"){
                mergeAtoms(atom,nearAtom,true );
                nearItems = scene->items(atom->x()-dist,atom->y()-dist,atom->boundingRect().width()+dist*2,
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

void ImageReader::mergeAtoms(AtomGraphicItem* a,AtomGraphicItem* a2, bool ord ){
    AtomGraphicItem* atom=a;
    AtomGraphicItem* atom2=a2;
    QString atomicSymbol = QString::fromStdString(a->getElement());
//    if(!ord && a->getElement()=="C"){
//        atom=a2;
//        atom2=a;
//    }
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
            scene->addBond(mergeAtoms[0],mergeAtoms[1],mergeBond->getBondOrder() );
        }
    }
    atom->setElement(atomicSymbol);
    scene->removeAtom(atom2);
    return;
}

QPointF* ImageReader::intersectingBonds(BondGraphicsItem* b1,BondGraphicsItem* b2,qreal minThresh,qreal maxThresh){
    QPointF* inter = new QPointF(-1,-1);

    //if lines cross
    if(b1->line().intersect(b2->line(),inter)==QLineF::BoundedIntersection){
        return  inter;
    }

    //distance between bonds start and end point
    qreal dist[4];
    dist[0]=pointLineDist(b1->line(), b2->line().p1());
    dist[1]=pointLineDist(b1->line(), b2->line().p2());
    dist[2]=pointLineDist(b2->line(), b1->line().p1());
    dist[3]=pointLineDist(b2->line(), b1->line().p2());

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

    //if lines aren't parallel and intercect point is close enought to one of the bonds return intercect point
    if(b1->line().intersect(b2->line(),inter)!=QLineF::NoIntersection){
        if(pointLineDist(b1->line(),*inter)<minDist || pointLineDist(b2->line(),*inter)<minDist )
            return inter;
    }

    //if lines are parallel return (-1,-1)
    inter->setX(-1);
    inter->setY(-1);
    return inter;
}

//Connects gap between bond endpoints if they are close enought
void ImageReader::combBonds(qreal minThresh,qreal maxThresh){
    int bondSize=bonds->size();
    for(int i=0;i<bondSize;i++){
        for(int j=i;j<bondSize;j++){
            if(!bonds->at(i)->hasSameAtoms(bonds->at(j))){
                QPointF* inter= intersectingBonds(bonds->at(i),bonds->at(j),minThresh,maxThresh);
                //if bonds intercet
                if (inter->x()!=-1){
                    AtomGraphicItem* atom = new AtomGraphicItem( *inter ,std::string("C"),0,1000);
                    AtomGraphicItem** ba_i =  bonds->at(i)->getAtoms();
                    AtomGraphicItem** ba_j =  bonds->at(j)->getAtoms();

                    //distance between intercection point and bond end points
                    qreal dist_i1= (atom->pos() - ba_i[0]->pos()).manhattanLength();
                    qreal dist_i2= (atom->pos() - ba_i[1]->pos()).manhattanLength();

                    qreal dist_j1=(atom->pos() - ba_j[0]->pos()).manhattanLength();
                    qreal dist_j2=(atom->pos() - ba_j[1]->pos()).manhattanLength();

                    //add new bond to atoms furthest appart
                    if(dist_i1<dist_i2){
                        scene->addBond(atom,bonds->at(i)->getAtoms()[1] );
                    }
                    else{
                        scene->addBond(atom,bonds->at(j)->getAtoms()[0] );
                    }

                    if(dist_j1<dist_j2){
                        scene->addBond(atom,bonds->at(j)->getAtoms()[1] );
                    }
                    else{
                        scene->addBond(atom,bonds->at(j)->getAtoms()[0] );
                    }
                    qDebug()<<*inter<<"inter"<<dist_i1<<dist_i2<<dist_j1<<dist_j2;
                    //add atom to scene and atom container
                    atoms->append(atom);
                    scene->addItem(atom);
                }
                delete inter;
            }
        }
    }

    for (int i=0;i<atoms->size();i++){
        mergeNearAtoms(atoms->at(i),2);
    }
    scene->clearOrphanedAtoms();
}

//merge lines that are close each other and almost paralell
std::vector<std::array<double, 4> > ImageReader::mergeLines(std::vector<std::array<double, 4> > lines){
    for (int i=0;i<lines.size();i++){
        QLineF l_i=QLineF(lines[i][0], lines[i][1], lines[i][2], lines[i][3]);
        for (int j=i+1;j<lines.size();j++){
            QLineF l_j=QLineF(lines[j][0], lines[j][1], lines[j][2], lines[j][3]);
            if( ( l_i.angleTo(l_j)<10 && l_i.angleTo(l_j)>0 )
              || (l_j.angleTo(l_i)<10 && l_j.angleTo(l_i)>0 ) )
            {
                if(lineDistance(l_i,l_j)<15 )
                {
                    if(l_i.length()>l_j.length()){
                        QLineF tmpLine=combineLines(l_i,l_j);
                        lines[i]=std::array<double,4>{tmpLine.x1(),tmpLine.y1(),tmpLine.x2(),tmpLine.y2() };
                        lines.erase(lines.begin()+j);
                        j--;
                    }
                    else{
                        QLineF tmpLine=combineLines(l_j,l_i);
                        lines[j]=std::array<double,4>{tmpLine.x1(),tmpLine.y1(),tmpLine.x2(),tmpLine.y2() };
                        lines.erase(lines.begin()+i);
                        i--;
                        j--;
                        if(i>=0)
                            l_i=QLineF(lines[i][0], lines[i][1], lines[i][2], lines[i][3]);
                        else {
                            continue;
                        }
                    }
                }
            }
        }
    }
    return lines;
}

void ImageReader::mergeLines(){
    for (int i=0;i<bonds->size()-1;i++){
        QLineF l_i=bonds->at(i)->line();
        for (int j=i+1;j<bonds->size();j++){
            QLineF l_j=bonds->at(j)->line();
            if( l_i.angleTo(l_j)<15 || l_j.angleTo(l_i)<15)
            {
                if(lineDistance(l_i,l_j )<5)
                {
                    if(l_i.length()>l_j.length())
                    {
                        bonds->at(i)->setLine(combineLines(l_i,l_j));
                        scene->removeBond(bonds->at(j));;
                        j--;
                    }
                    else
                    {
                        bonds->at(j)->setLine(combineLines( l_j,l_i));
                        scene->removeBond(bonds->at(i));
                        i--;
                        if(bonds->size()<i)
                            l_i=bonds->at(i)->line();
                        else
                            break;
                    }
                }
            }
        }
    }
    return;
}


void ImageReader::mergeNearBonds(BondGraphicsItem* bond)
{
    BondGraphicsItem* tmp;
    QLineF l_i=bond->line();
    for (int j=0;j<bonds->size();j++){
        if(bonds->at(j)==bond)
            continue;
        QLineF l_j=bonds->at(j)->line();
        if( l_i.angleTo(l_j)<15 || l_j.angleTo(l_i)<15)
        {
           if(lineDistance(l_i,l_j )<15 )
            {
                if(l_i.length()>l_j.length()){
                    bond->setLine(combineLines(l_i,l_j) );
                    tmp=bonds->at(j);
                    scene->removeBond(tmp);
                    j--;
                }
                else{
                    bonds->at(j)->setLine(combineLines(l_i,l_j) );
                    scene->removeBond(bond);
                    return;
                }
            }
        }
    }
    return;
}

//combines two line segments
QLineF ImageReader::combineLines(QLineF l1,QLineF l2){
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
    if( (l1.angleTo(nLine) <340 && l1.angleTo(nLine) > 20) || (l2.angleTo(nLine) <340 && l2.angleTo(nLine) > 20)){
        nLine.setAngle(nLine.angle()+180);
    }
    return nLine;
}

//finds minumum distance between two line segments
qreal ImageReader::lineDistance(QLineF l1, QLineF l2){
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
qreal ImageReader::pointLineDist(QLineF v, QPointF p ){
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
    return sqrt(dx*dx+dy*dy);
}



void ImageReader::removeAtomsNearBonds(){
    std::sort(atoms->begin(),atoms->end(), scene->compareAtoms );
    //remove bonds that overlap atom
    for(int j=0;j<atoms->size();j++){
        AtomGraphicItem* atom=atoms->at(j);
        qreal dist = 5;
        QList<QGraphicsItem*> nearItems = scene->items(atom->x()-dist,atom->y()-dist,atom->boundingRect().width()+dist*2,atom->boundingRect().height()+dist*2,  Qt::IntersectsItemShape, Qt::AscendingOrder	 ,QTransform());
        for (int i=0;i<nearItems.size();i++){
            if(nearItems[i]->type()==BondGraphicsItem::Type){
                BondGraphicsItem* nearBond=dynamic_cast<BondGraphicsItem*>(nearItems[i]);
                AtomGraphicItem** bondAtoms = nearBond->getAtoms();
                if((bondAtoms[0]!=atom) &&(bondAtoms[1]!=atom )){
                    qDebug()<<"Removed atom near bond"<<j<<i;
                    //nearBond->setBondOrder(3);
                    scene->removeAtom(atom);
                    //i--;
                    //removeBond(nearBond);
                    j--;
                    break;
                }
            }
        }
    }
    return;
}

