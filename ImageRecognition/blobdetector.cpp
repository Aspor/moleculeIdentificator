#include "blobdetector.h"

using namespace cv;

BlobDetector::BlobDetector()
{

    SimpleBlobDetector::Params params;
    //params.minThreshold=0;
    //params.maxThreshold=500;
    //params.thresholdStep=2;
    params.filterByArea=true;
    //params.minArea=15;

    params.filterByCircularity = false;
    params.filterByConvexity = false;
    params.filterByInertia = false;
    detector = SimpleBlobDetector (params);
}

vector<KeyPoint> BlobDetector::detectBlobs(cv::Mat src){

    Mat im;
    vector<KeyPoint> keyPoints;
  //  src=imread(file);
   // GaussianBlur(src, im, Size(15,15),7,7);
    GaussianBlur( src, im, Size(5, 5), 17, 17 );

    detector.detect(im,keyPoints);
    return keyPoints;
}

std::vector<std::array < int,2 > > BlobDetector::getBlobPos(Mat src){
    vector<KeyPoint> KeyPoints=detectBlobs(src);
    std::vector<std::array< int,2 > > positions;
    for (int i=0;i<KeyPoints.size();i++) {
        KeyPoint point = KeyPoints[i];
        std::array<int,2> pos;
        pos [0]= point.pt.x;
        pos [1]= point.pt.y;
        positions.push_back(pos);
    }
    return positions;
}
