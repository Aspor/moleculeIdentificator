#include "atomfinder.h"
#include<qdebug.h>


using namespace cv;

atomFinder::atomFinder()
{
    ocr=new OCR();

}

std::vector<AtomGraphicItem *> atomFinder::labelAtoms(std::string file, std::vector<std::string> atomsSymbols){
    Mat src = imread(file,IMREAD_COLOR );
    findBlobs( src);
    qDebug()<<"countours.size"<<countours.size()<<atoms.size();
    matchAtomPos( );
    qDebug()<<"countours.size"<<countours.size();


    for(int i=0;i<countours.size();i++){
        std::string label = labelBlob(countours[i],src);
        //if(label.compare(" ")){
        Rect b = boundingRect( countours[i]);
        atoms.push_back(new AtomGraphicItem(QPoint(b.x,b.y), label,0,i+100 ));
    }
    return atoms;
}

void atomFinder::findBlobs(Mat src){
    Mat img, canny_output;
    cvtColor( src, img, COLOR_BGR2GRAY );
    //blur(img,img,Size(10,10));
    //Canny( img, canny_output, 0, 20, 3 );
    bitwise_not(img,img);

    canny_output=canny_output.zeros(img.size(),CV_32F);

    vector<Vec4i> hierarchy;
    findContours(img,countours, hierarchy, RETR_TREE, CHAIN_APPROX_NONE, Point(0,0) );

}

void atomFinder::matchAtomPos(){

    double avrgArea=0;
    int numOfCountours=countours.size();
    for (int i=0; i<numOfCountours;i++){
        qDebug()<<boundingRect(countours[i]).area()<<"AREA";
        avrgArea+=boundingRect(countours[i]).area()/numOfCountours;
    }
    qDebug()<<avrgArea<<"avrgArea";


    for (int i=0; i<countours.size();i++){
        Rect rect=boundingRect(countours[i]);
        double area=rect.area();
        if(area>avrgArea || rect.width/rect.height>10 || rect.height/rect.width>10  ){
            countours.erase(countours.begin()+i);
            i--;
        }
    }
    //    for (int i=0; i<countours.size()-1;i++){
    for (int i=0; i<countours.size()-1;i++){
        Rect rect1=boundingRect(countours[i]);
        bool comb=false;
        for (int j=i+1; j<countours.size();j++){
            Rect rect2=boundingRect(countours[j]);
            //int dist=norm(rect1.tl-rect2.tl);
            int xDist=abs( rect1.x-rect2.x)-5;
            int yDist=abs(rect1.y-rect2.y)-5;

            if((xDist<rect1.width || xDist<rect2.width) && (yDist<rect1.height||yDist<rect2.height)){
            //if((norm(rect1.tl()-rect2.tl()) < rect1.width+rect2.width-15 ) || (norm( rect1.tl()-rect2.tl()) < rect1.height+rect2.height-15 )){
                vector<Point> count;
                hconcat(countours[i],countours[j],count);
             //if(count.size()<countours[i].size()+countours[j].size()){
                qDebug()<< countours[i].size()<<countours[j].size() <<count.size()<<"count"<<i<<j;
                comb=true;

                countours[i]=count;
                rect1=boundingRect(countours[i]);
                countours.erase(countours.begin()+j);
                //countours.insert(countours.begin(),count);
                j--;

            }
        }
//        if(comb)
//            i--;
    }
    qDebug()<<"COMB countours size"<<countours.size();
}





std::string atomFinder::labelBlob(cv::vector<Point> points,Mat src){
    //TODO OCR;
    Rect bound = boundingRect(points);
    Mat mat(src,bound);
    //mat = Mat::zeros(bound.size(),CV_8UC1);



//    int x0=bound.x;
//    int y0=bound.y;
//    for(int i=0;i<points.size();i++){
//        //mat.at<bool>(   points[i].y -y0 , points[i].x -x0 )=1;
//        mat.ptr( points[i].y -y0 )[ points[i].x -x0]=255 ;

//       // mat.at<int>(  points[i].y -y0, points[i].x -x0)=INTMAX_MAX ;
//    }


    int s=  sum(mat)[0];
    std::stringstream ss;
    ss << s;

    return ocr->ocr(mat);
    return "D";
}


