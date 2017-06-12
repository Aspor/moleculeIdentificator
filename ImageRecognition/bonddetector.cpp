#include "bonddetector.h"
#include<QDebug>

bondDetector::bondDetector()
{

}
std::vector<std::array<int,4> > bondDetector::detectEdges(std::string file )
{
    cv::Mat src, dst ;
    src=cv::imread(file,0);
    //  Laplacian(src,dst,5,1);

    std::vector<cv::Vec4i> lines;


//  cv::blur(dst,dst,cv::Size(10,10));
//    cv::Canny( dst, dst, 10, 22,3);
    cv::blur(src,src,cv::Size(5,5));
    cv::blur(src,src,cv::Size(5,5));


    cv::adaptiveThreshold(src,dst, 255, cv::ADAPTIVE_THRESH_GAUSSIAN_C ,0  , 155, 0);



    cv::bitwise_not(dst,dst);
    cv::HoughLinesP( dst, lines, 1, CV_PI/90,35,25, 25);



    //THIS
    //cv::HoughLinesP( dst, lines, 1, CV_PI/90, 65,50, 3);


    std::vector<std::array<int,4> > bonds;
    qDebug()<<"lines"<<lines.size();

    //Delete lines that are too near each other
//    for(size_t i =0;i<lines.size()-1;i++){
//        cv::vector<int> P1={lines[i][0],lines[i][1]};
//        cv::vector<int> P2= {lines [i][2],lines[i][3]};
//        for(size_t j=i+1;j<lines.size();j++){
//            int x=lines[j][0];
//            int y=lines[j][1];

//            //start or end points are near
//            if((abs( P1[0]-x)<10 && abs(P1[1]-y)<10) || (abs(P2[0]-x)<10 && abs(P2[1]-y)<10)){
//                qDebug()<<"END START";
//                continue;
//            }

//            double distance = abs(P2[1]-P1[1]*x - (P2[1]-P1[1])*y +(P2[2]*P1[2] - P1[0]*P2[1]) ) /
//                                                    sqrt( pow( (P1[1]-P2[1]),2)  + pow((P1[0]-P2[0]),2));
//            if(distance<20){
//                lines.erase(lines.begin()+j);
//                j--;
//                continue;
//            }
//            x=lines[j][2];
//            y=lines[j][3];

//            if((abs( P1[0]-x)<10 && abs(P1[1]-y)<10) || (abs(P2[0]-x)<10 && abs(P2[1]-y)<10)){
//                qDebug()<<"START END";
//                continue;
//            }

//            distance = abs(P2[1]-P1[1]*x - (P2[1]-P1[1])*y +(P2[2]*P1[2] - P1[0]*P2[1]) ) /
//                                                    sqrt( pow( (P1[1]-P2[1]),2)  + pow((P1[0]-P2[0]),2));
//            if(distance<20){
//                lines.erase(lines.begin()+j);
//                j--;
//                continue;
//            }
//        }
//    }
    qDebug()<<"LINES SIZE"<<lines.size();
//    int lenght=0;
//    for( size_t i = 0; i < lines.size(); i++ )
//     {
//        lenght+=( abs( lines[i][0]-lines[i][2] )+ abs( lines[i][1]-lines[i][3]));
//     }
//    int avrgLenght=lenght/lines.size();
//    qDebug()<<"avrgLenght"<<avrgLenght;
    for( size_t i = 0; i < lines.size(); i++ )
     {
//       lenght = abs( lines[i][0]-lines[i][2] )+ abs( lines[i][1]-lines[i][3]);
//       if(lenght<avrgLenght*2){
//           qDebug()<<"Len"<<lenght<<avrgLenght;
         std::array<int,4> line={lines[i][0] ,lines[i][1],lines[i][2],lines[i][3]};
         bonds.push_back(line);
       }
     //}
     qDebug()<<"BONDSSize"<<bonds.size();
     return bonds;
}

