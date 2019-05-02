#include "bonddetector.h"
#include<QDebug>

BondDetector::BondDetector()
{

}
std::vector<std::array<double,4> > BondDetector::detectEdges(cv::Mat src )
{
    cv::Mat  dst,im, cdst ;
    //cv::cvtColor( src, im, cv::COLOR_RGBA2GRAY );
    std::vector<cv::Vec4d> lines;
    im=src;

    cv::adaptiveThreshold(im,dst,255,cv::ADAPTIVE_THRESH_GAUSSIAN_C,cv::THRESH_BINARY_INV,255,33);

    cv::HoughLinesP( dst, lines, 1, CV_PI/180, 25, 100,5 );

    std::vector<std::array<double,4> > bonds;
    for( size_t i = 0; i < lines.size(); i++ )
     {
         std::array<double,4> line={lines[i][0] ,lines[i][1],lines[i][2],lines[i][3]};
         bonds.push_back(line);
     }
     return bonds;
}

