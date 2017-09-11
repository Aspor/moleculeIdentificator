#include "moleculegrabber.h"

#include <qdebug.h>


MoleculeGrabber::MoleculeGrabber()
{

}


cv::Mat MoleculeGrabber::grabMolecule(std::string filename){
    cv::vector<cv::vector <cv::Point> >contours;
    cv::Mat threshOut;

    cv::Mat src = cv::imread(filename,cv::IMREAD_COLOR);
    cv::Mat img,srcBW,im;

    if(src.rows>1000)
        cv::resize(src,im,cv::Size(),0.1,0.1);
    else
        im=src;

    cv::cvtColor( im, srcBW, cv::COLOR_BGR2GRAY );
    cv::bilateralFilter(srcBW,img,50,75,5);
    cv::adaptiveThreshold(img,threshOut,255,cv::ADAPTIVE_THRESH_GAUSSIAN_C,1,125,0);
    cv::Mat kernel =cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(15,15));
    cv::dilate(threshOut,threshOut,kernel);
    cv::findContours(threshOut,contours, CV_RETR_LIST ,cv::CHAIN_APPROX_NONE,cv::Point(0,0));
    cv::vector<cv::Rect> boundRects(contours.size());
    cv::Point imgCenter=cv::Point(im.cols/2,im.rows/2);
    double minDist=INT_MAX;
    int nearInd;
    for(int i=0;i<contours.size();i++){
        boundRects[i]=cv::boundingRect (cv::Mat(contours[i]));
        cv::Point center=(boundRects[i].tl()+boundRects[i].br())*0.5;
        cv::Point diff=center-imgCenter;
        double dist =sqrt((diff.x*diff.x + diff.y*diff.y));
        if(dist<minDist){
            minDist=dist;
            nearInd=i;
        }
    }

    cv::Mat mol(im,boundRects[nearInd] );
    if(src.rows>1000) cv::resize(mol,mol,cv::Size(),2,2);
//    cv::namedWindow( "aoe", cv::WINDOW_AUTOSIZE );
//    cv::imshow( "aoe", mol );

//    cv::waitKey(0);


    return mol;
}
