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
    //im=src;

    cv::bilateralFilter(src,im,5,100,100,cv::BORDER_REFLECT);
    qDebug()<<cv::mean(im)[0]<<" mean "<<cv::mean(im)[1]<<cv::mean(im)[2]<<"  "<<cv::mean(im)[3];
    cv::adaptiveThreshold(im,dst,255,cv::ADAPTIVE_THRESH_GAUSSIAN_C,cv::THRESH_BINARY_INV,55,20);
    //cv::threshold(dst,im,125,255,cv::THRESH_BINARY);

    cv::Canny( dst, cdst, 50, 200, 3 );
//    cv::dilate(dst, dst, cv::Mat(), cv::Point(-1,-1), 1);
//    std::vector<cv::Vec4i> lines;
    cv::HoughLinesP( cdst, lines, 1, CV_PI/180, 95, 85,2 );


    qDebug()<<"LINES"<<lines.size();
    std::vector<std::array<double,4> > bonds;

//    cv::cvtColor(src, cdst, cv::COLOR_GRAY2BGR);
    cdst=src;

//    for( size_t i = 0; i < lines.size(); i++ )
//      {
//          cv::Vec4i l = lines[i];
//          cv::line( cdst, cv::Point(l[0], l[1]), cv::Point(l[2], l[3]), cv::Scalar(0,0,255), 3, cv::LINE_AA);
//      }


//    imshow("Detected Lines (in red) - Standard Hough Line Transform", cdst);
//    cv::waitKey(0);

    for( size_t i = 0; i < lines.size(); i++ )
     {
         std::array<double,4> line={lines[i][0] ,lines[i][1],lines[i][2],lines[i][3]};
         bonds.push_back(line);
     }
     return bonds;
}

