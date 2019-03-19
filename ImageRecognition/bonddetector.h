#ifndef BONDDETECTOR_H
#define BONDDETECTOR_H
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"


#include <vector>

//Finds lines from images

class BondDetector
{
public:
    BondDetector();

    std::vector<std::array<double,4> > detectEdges(cv::Mat);
};


#endif // BONDDETECTOR_H
