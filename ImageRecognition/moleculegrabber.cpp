#include "moleculegrabber.h"

#include <qdebug.h>


MoleculeGrabber::MoleculeGrabber()
{

}


cv::Mat MoleculeGrabber::grabMolecule(cv::Mat src){
    std::vector<std::vector <cv::Point> >contours;
    cv::Mat threshOut;
    cv::Mat img,srcBW,im, imGauss,lap;

//    //Scale image to fit
//    if(src.rows>1000)
//        cv::resize(src,im,cv::Size(),0.1,0.1);
//    else
        im=src;

    //cv::Canny( im, srcBW , 150, 400, 3 );
    cv::adaptiveThreshold(im,srcBW,255,cv::ADAPTIVE_THRESH_GAUSSIAN_C,cv::THRESH_BINARY_INV,255,33);

    cv::dilate(srcBW, srcBW, cv::Mat(), cv::Point(-1,-1), 13);
    cv::dilate(srcBW, srcBW, cv::Mat(), cv::Point(-1,-1), 13);
    cv::dilate(srcBW, srcBW, cv::Mat(), cv::Point(-1,-1), 13);

    cv::findContours(srcBW,contours, cv::RETR_LIST ,cv::CHAIN_APPROX_NONE,cv::Point(0,0));


    std::vector<cv::Rect> boundRects(contours.size());
    cv::Point imgCenter=cv::Point(im.cols/2,im.rows/2);
    double minDist=INT_MAX;
    int nearInd;
//    qDebug()<<"countours"<<contours.size();
    //Find countour that is closest to center
    for(int i=0;i<contours.size();i++){
        boundRects[i]=cv::boundingRect (cv::Mat(contours[i]));
        cv::Point center=(boundRects[i].tl()+boundRects[i].br())*0.5;
        cv::Point diff=center-imgCenter;
        double dist =sqrt((diff.x*diff.x + diff.y*diff.y));
        if(dist<minDist){
            //choose only contour that are large enought
//            if((boundRects[i].width*boundRects[i].height > 6000 )){
                minDist=dist;
//                qDebug()<<dist<<i<<"NEAR"<<boundRects[i].width*boundRects[i].height;
                nearInd=i;
//            }
        }
    }

    cv::Mat mol(im,boundRects[nearInd] );

//    cv::namedWindow( "image", cv::WINDOW_AUTOSIZE );
//    cv::imshow( "image", mol );
//    cv::waitKey(0);

//    if(src.rows>1000) cv::resize(mol,mol,cv::Size(),2,2);
//    cv::namedWindow( "imgMOL", cv::WINDOW_AUTOSIZE );
//    cv::imshow( "imgMOL", mol );

    return mol;
}
