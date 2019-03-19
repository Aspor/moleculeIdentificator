#include "atomfinder.h"
#include<qdebug.h>


using namespace cv;

AtomFinder::AtomFinder()
{
    ocr=new OCR();
}

std::vector<AtomGraphicItem *> AtomFinder::labelAtoms(cv::Mat src, std::vector<std::string> atomsSymbols){

    findBlobs( src);
    //qDebug()<<"countours.size"<<countours.size()<<atoms.size();
    matchAtomPos( );
    //qDebug()<<"countours.size"<<countours.size();
    for(int i=0;i<countours.size();i++){
        std::string label = labelBlob(countours[i],src);
        //if(label.compare(" ")){
        Rect b = boundingRect( countours[i]);
        atoms.push_back(new AtomGraphicItem(QPoint(b.x,b.y), label,0,i+100 ));
    }
    return atoms;
}

void AtomFinder::findBlobs(Mat src){
    Mat img, canny_output, dst;
    cvtColor( src, img, COLOR_BGR2GRAY );
    blur(img,img,Size(10,10));
    blur(img,img,Size(10,10));
    blur(img,img,Size(10,10));
    blur(img,img,Size(10,10));



    cv::bilateralFilter(img,dst,50,75,5);
    cv::adaptiveThreshold(dst,dst,255,cv::ADAPTIVE_THRESH_GAUSSIAN_C,1,205,0);



//    Canny( dst, canny_output, 0, 20, 3 );
    bitwise_not(img,img);

   // canny_output=canny_output.zeros(img.size(),CV_32F);

    std::vector<Vec4i> hierarchy;


    findContours(dst,countours, hierarchy,  cv::RETR_LIST , CHAIN_APPROX_NONE, Point(0,0) );
}

//Try to find elements from list of countours
void AtomFinder::matchAtomPos(){

    double avrgArea=0;
    int numOfCountours=countours.size();
    qDebug()<<"atom countours"<<countours.size();
    //Calculate avarange area of countours.
    for (int i=0; i<numOfCountours;i++){
        qDebug()<<boundingRect(countours[i]).area()<<"AREA";
        avrgArea+=boundingRect(countours[i]).area()/numOfCountours;
    }
    qDebug()<<avrgArea<<"avrgArea";
    //Remove countours that are larger than avarange or not square enought
    //Skeleton is among countours and it should be removed.
    for (int i=0; i<countours.size();i++){
        Rect rect=boundingRect(countours[i]);
        double area=rect.area();
        if(area>avrgArea || rect.width/rect.height>10 || rect.height/rect.width>10  ){
            countours.erase(countours.begin()+i);
            i--;
        }
    }
    //Combine countours that are near each other
    for (int i=0; i<countours.size()-1;i++){
        Rect rect1=boundingRect(countours[i]);
        bool comb=false;
        for (int j=i+1; j<countours.size();j++){
            Rect rect2=boundingRect(countours[j]);
            int xDist=abs( rect1.x-rect2.x)-5;
            int yDist=abs(rect1.y-rect2.y)-5;
            if((xDist<rect1.width || xDist<rect2.width) && (yDist<rect1.height||yDist<rect2.height)){
                std::vector<Point> count;
                hconcat(countours[i],countours[j],count);
                //qDebug()<< countours[i].size()<<countours[j].size() <<count.size()<<"count"<<i<<j;
                comb=true;
                countours[i]=count;
                rect1=boundingRect(countours[i]);
                countours.erase(countours.begin()+j);
                j--;

            }
        }
    }
}

//Optical character regognition
std::string AtomFinder::labelBlob(std::vector<Point> points,Mat src){
    Rect bound = boundingRect(points);
    Mat mat(src,bound);
    return ocr->ocr(mat);
}


