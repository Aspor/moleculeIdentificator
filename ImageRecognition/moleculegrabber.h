#ifndef MOLECULEGRABBER_H
#define MOLECULEGRABBER_H

#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

//Finds blobs and chooses one that is nearest of center

class MoleculeGrabber
{
public:
    MoleculeGrabber();
    cv::Mat grabMolecule(cv::Mat src);

};

#endif // MOLECULEGRABBER_H
