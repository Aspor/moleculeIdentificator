#ifndef BLOBDETECTOR_H
#define BLOBDETECTOR_H

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/opencv.hpp"


class BlobDetector
{
public:
    BlobDetector();
    std::vector<cv::KeyPoint>detectBlobs(std::string file);
    std::vector<std::array <int,2> >getBlobPos(std::string file);
private:
    cv::SimpleBlobDetector detector;
};

#endif // BLOBDETECTOR_H
