#ifndef MOLECULECLEANER_H
#define IMAGEREADER_H

#include <QObject>
#include "atomgraphicitem.h"
#include "bondgraphicsitem.h"
#include <vector>
#include <algorithm>
#include <QGraphicsScene>

#include <ImageRecognition/atomfinder.h>
#include <ImageRecognition/bonddetector.h>
#include <ImageRecognition/moleculegrabber.h>

class EditorScene;

class ImageReader
{
public:
    ImageReader(EditorScene* parent);
    void read(cv::Mat byteImg, QVector<AtomGraphicItem *> *a, QVector<BondGraphicsItem *> *b);
private:
    EditorScene* scene;
    QVector<AtomGraphicItem*> *atoms;
    QVector<BondGraphicsItem*> *bonds;

    void clean(QVector<AtomGraphicItem *> *a, QVector<BondGraphicsItem *> *b);

    bool mergeNearAtoms(AtomGraphicItem *atom,int dist=20);
    void mergeAtoms(AtomGraphicItem* a,AtomGraphicItem* a2, bool ord=false );
    QPointF* intersectingBonds(BondGraphicsItem* b1,BondGraphicsItem* b2,qreal minThresh=20,qreal maxThresh=50);
    void combBonds(qreal minThresh=20,qreal maxThres=50);
    std::vector<std::array<double, 4> > mergeLines(std::vector<std::array<double, 4> > lines);
    void mergeLines();
    void mergeNearBonds(BondGraphicsItem* bond);
    QLineF combineLines(QLineF l1,QLineF l2);
    qreal lineDistance(QLineF l1, QLineF l2);
    qreal pointLineDist(QLineF v, QPointF p );
    void removeAtomsNearBonds();

};

#endif // MOLECULECLEANER_H
