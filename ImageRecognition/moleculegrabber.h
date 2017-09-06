#ifndef MOLECULEGRABBER_H
#define MOLECULEGRABBER_H

#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv/cv.h"

class MoleculeGrabber
{
public:
    MoleculeGrabber();
    cv::Mat grabMolecule(std::string filename);

};

#endif // MOLECULEGRABBER_H
