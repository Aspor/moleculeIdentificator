#include "bonddetector.h"
#include<QDebug>

bondDetector::bondDetector()
{

}
std::vector<std::array<int,4> > bondDetector::detectEdges(std::string file )
{
    cv::Mat src, dst, color_dst;
    src=cv::imread(file);
    cv::Canny( src, dst, 50, 200);
    //  Laplacian(src,dst,5,1);
    cv::cvtColor( dst, color_dst, CV_GRAY2BGR );
    std::vector<cv::Vec4i> lines;
    cv::HoughLinesP( dst, lines, 1, CV_PI/180, 15, 45, 15 );
    std::vector<std::array<int,4> > bonds;
    qDebug()<<"lines"<<lines.size();
    for( size_t i = 0; i < lines.size(); i++ )
     {

       std::array<int,4> line={lines[i][0] ,lines[i][1],lines[i][2],lines[i][3]};
       bonds.push_back(line);

     }
     return bonds;
}
