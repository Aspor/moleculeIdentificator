#ifndef BONDDETECTOR_H
#define BONDDETECTOR_H


#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv/cv.h"

#include <vector>

class bondDetector
{
public:
    bondDetector();

    std::vector<std::array<int,4> > detectEdges(std::string);
};


#endif // BONDDETECTOR_H
