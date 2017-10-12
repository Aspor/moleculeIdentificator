#ifndef BONDDETECTOR_H
#define BONDDETECTOR_H
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv/cv.h"

#include <vector>

//Finds lines from images

class bondDetector
{
public:
    bondDetector();

    std::vector<std::array<int,4> > detectEdges(cv::Mat);
};


#endif // BONDDETECTOR_H
