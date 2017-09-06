#ifndef BLOBDETECTOR_H
#define BLOBDETECTOR_H

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/opencv.hpp"


class BlobDetector
{
public:
    BlobDetector();
    std::vector<cv::KeyPoint>detectBlobs(cv::Mat src);
    std::vector<std::array <int,2> >getBlobPos(cv::Mat src);
private:
    cv::SimpleBlobDetector detector;
};

#endif // BLOBDETECTOR_H
