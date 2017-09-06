#include "bonddetector.h"
#include<QDebug>

bondDetector::bondDetector()
{

}
std::vector<std::array<int,4> > bondDetector::detectEdges(cv::Mat src )
{
    cv::Mat  dst,im ;
    cv::cvtColor( src, im, cv::COLOR_BGR2GRAY );

    //  Laplacian(src,dst,5,1);

    std::vector<cv::Vec4i> lines;


//  cv::blur(dst,dst,cv::Size(10,10));
//    cv::Canny( dst, dst, 10, 22,3);
//    cv::blur(src,src,cv::Size(5,5));
//    cv::blur(src,src,cv::Size(5,5));
 //   cv::boxFilter(src,src,-1,cv::Size(4,4));

    cv::bilateralFilter(im,dst, 50,75,5);

    cv::adaptiveThreshold(dst,dst, 255, cv::ADAPTIVE_THRESH_GAUSSIAN_C ,0  , 125, 0);

    cv::bitwise_not(dst,dst);
    //cv::HoughLinesP( dst, lines, 1, CV_PI/90,35,25, 25);

    cv::HoughLinesP( dst, lines, 1, CV_PI/90,45,5, 10);


    //THIS
    //cv::HoughLinesP( dst, lines, 1, CV_PI/90, 65,50, 3);

    std::vector<std::array<int,4> > bonds;
    for( size_t i = 0; i < lines.size(); i++ )
     {
         std::array<int,4> line={lines[i][0] ,lines[i][1],lines[i][2],lines[i][3]};
         bonds.push_back(line);
     }
     qDebug()<<"BONDSSize"<<bonds.size();
     return bonds;
}

