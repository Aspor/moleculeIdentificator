#ifndef ATOMFINDER_H
#define ATOMFINDER_H
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include <atomgraphicitem.h>
#include "ocr.h"


//Find blobs rom image and laber them

class AtomFinder
{
public:
    AtomFinder();
    void findBlobs(cv::Mat img);
    void matchAtomPos(std::vector<std::array<int,2> > atomPos );
    void matchAtomPos();

    std::string labelBlob(std::vector<cv::Point>,cv::Mat src);
    std::vector< AtomGraphicItem*> labelAtoms(cv::Mat src, std::vector<std::string> atoms,double scale);

private:
    std::vector<std::vector< cv::Point> > countours;
    std::vector<AtomGraphicItem* > atoms;
    std::vector<std::array<int,2> > pos;

    OCR* ocr;

    //TODO SOM som;
};

#endif // ATOMFINDER_H
