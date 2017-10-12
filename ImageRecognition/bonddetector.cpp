#include "bonddetector.h"
#include<QDebug>

bondDetector::bondDetector()
{

}
std::vector<std::array<int,4> > bondDetector::detectEdges(cv::Mat src )
{
    cv::Mat  dst,im ;
    cv::cvtColor( src, im, cv::COLOR_BGR2GRAY );
    std::vector<cv::Vec4i> lines;

//    cv::namedWindow( "bond", cv::WINDOW_AUTOSIZE );
//    cv::imshow( "bond", src );


    //blur and threshold to remove lines from grid paper
    cv::blur(im,dst,cv::Size(10,10));
    cv::adaptiveThreshold(dst,dst, 255, cv::ADAPTIVE_THRESH_GAUSSIAN_C ,1  , 125, 0);
//    cv::Mat kernel =cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(3,3));
//    cv::dilate(dst,dst,kernel);
    qDebug()<<"BONDS_";

    cv::HoughLinesP( dst, lines, 1, CV_PI/90,5,25, 5);
    //cv::HoughLinesP(dst,lines,1,CV_PI/90,75,5,5);
    std::vector<std::array<int,4> > bonds;
    qDebug()<<"BONDS"<<lines.size();
    for( size_t i = 0; i < lines.size(); i++ )
     {
         std::array<int,4> line={lines[i][0] ,lines[i][1],lines[i][2],lines[i][3]};
         bonds.push_back(line);
     }
     qDebug()<<"BONDSSize"<<bonds.size();
     return bonds;
}
