#include "bonddetector.h"
#include<QDebug>

bondDetector::bondDetector()
{

}
std::vector<std::array<int,4> > bondDetector::detectEdges(std::string file )
{
    cv::Mat src, dst;
    src=cv::imread(file,0);
    //cv::Canny( src, dst, 50, 200);
    //cv::Canny( src, dst, 675, 750);
    cv::bitwise_not(src,dst);
    //  Laplacian(src,dst,5,1);
    std::vector<cv::Vec4i> lines;
    cv::HoughLinesP( dst, lines, 1, CV_PI/180, 45, 30, 5 );
    //cv::HoughLinesP( dst, lines, 1, CV_PI/180, 35, 175, 175);
    std::vector<std::array<int,4> > bonds;
//    qDebug()<<"lines"<<lines.size();
//    for(size_t i =0;i<lines.size()-1;i++){
//        cv::vector<int> P1={lines[i][0],lines[i][1]};
//        cv::vector<int> P2= {lines [i][2],lines[i][3]};
//        for(size_t j=i;j<lines.size();j++){
//            int x=lines[j][0];
//            int y=lines[j][1];
//            double distance = abs(P2[1]-P1[1]*x - (P2[1]-P1[1])*y +(P2[2]*P1[2] - P1[0]*P2[1]) ) /
//                                                    sqrt( pow( (P1[1]-P2[1]),2)  + pow((P1[0]-P2[0]),2));
//            if(distance<5){
//                lines.erase(lines.begin()+j);
//                j--;
//                continue;
//            }
//            x=lines[j][2];
//            y=lines[j][3];
//            distance = abs(P2[1]-P1[1]*x - (P2[1]-P1[1])*y +(P2[2]*P1[2] - P1[0]*P2[1]) ) /
//                                                    sqrt( pow( (P1[1]-P2[1]),2)  + pow((P1[0]-P2[0]),2));
//            if(distance<5){
//                lines.erase(lines.begin()+j);
//                j--;
//                continue;
//            }
//        }
//    }

    for( size_t i = 0; i < lines.size(); i++ )
     {
       std::array<int,4> line={lines[i][0] ,lines[i][1],lines[i][2],lines[i][3]};
       bonds.push_back(line);
     }
     return bonds;
}
