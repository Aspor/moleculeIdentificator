#include "atomfinder.h"
#include<qdebug.h>


using namespace cv;

atomFinder::atomFinder()
{

}

std::vector<AtomGraphicItem *> atomFinder::labelAtoms(std::__cxx11::string file, std::vector<std::__cxx11::string> atomsSymbols){
    Mat src = imread(file,IMREAD_COLOR );
    findBlobs( src);
    qDebug()<<"countours.size"<<countours.size()<<atoms.size();
    matchAtomPos( );
    qDebug()<<"countours.size"<<countours.size();


    for(int i=0;i<countours.size();i++){


        std::string label = labelBlob(countours[i]);
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

    vector<Vec4i> hierarchy;
    findContours(img,countours, hierarchy, RETR_TREE, CHAIN_APPROX_NONE, Point(0,0) );

}

//void atomFinder::matchAtomPos(std::vector<std::array<int,2> > atomPos){

//    double avrgArea=0;
//    int numOfCountours=countours.size();
//    for (int i=0; i<numOfCountours;i++){
//        qDebug()<<boundingRect(countours[i]).area()<<"AREA";
//        avrgArea+=boundingRect(countours[i]).area()/numOfCountours;
//    }
//    qDebug()<<avrgArea<<"avrgArea";


//    for (int i=0; i<countours.size();i++){
//        double area=boundingRect(countours[i]).area();
//        if(area>avrgArea){
//            countours.erase(countours.begin()+i);
//            i--;
//        }
//    }
//    //    for (int i=0; i<countours.size()-1;i++){
//    for (int i=0; i<countours.size()-1;i++){
//        Rect rect1=boundingRect(countours[i]);
//        bool comb=false;

//        for (int j=i+1; j<countours.size();j++){
//            Rect rect2=boundingRect(countours[j]);
//            if(norm(rect1.tl()-rect2.tl()) < rect1.width+rect2.width ){
//                vector<Point> count;
//                hconcat(countours[i],countours[j],count);
//                qDebug()<< countours[i].size()<<countours[j].size() <<count.size();
//                comb=true;
//                countours.insert(countours.begin(), count);
////                countours.erase(countours.begin()+j);
//                j++;
//                i++;
//            }
//        }
////        if(comb)
////            i--;
//    }
//    qDebug()<<"COMB countours size"<<countours.size();

//    for (int i=0; i<countours.size();i++){
//        bool isAtom=false;
//        for (int j=0;j<atomPos.size();j++){
//            if(boundingRect(countours[i]).contains(Point(atomPos[j][0],atomPos[j][1]) )){
//                isAtom=true;
//            }
//        }
//        if(!isAtom){
//            countours.erase(countours.begin()+i);
//            i--;
//        }
//    }

//}

void atomFinder::matchAtomPos(){

    double avrgArea=0;
    int numOfCountours=countours.size();
    for (int i=0; i<numOfCountours;i++){
        qDebug()<<boundingRect(countours[i]).area()<<"AREA";
        avrgArea+=boundingRect(countours[i]).area()/numOfCountours;
    }
    qDebug()<<avrgArea<<"avrgArea";


    for (int i=0; i<countours.size();i++){
        double area=boundingRect(countours[i]).area();
        if(area>avrgArea){
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
            if((norm(rect1.tl()-rect2.tl()) < rect1.width+rect2.width ) || (norm( rect1.tl()-rect2.tl()) < rect1.height+rect2.height )){
                vector<Point> count;
                hconcat(countours[i],countours[j],count);
                qDebug()<< countours[i].size()<<countours[j].size() <<count.size();
                comb=true;
                countours.insert(countours.begin(), count);
//                countours.erase(countours.begin()+j);
                j++;
                i++;
            }
        }
//        if(comb)
//            i--;
    }
    qDebug()<<"COMB countours size"<<countours.size();
}





std::string atomFinder::labelBlob(cv::vector<Point> points){
    //TODO OCR;
    Rect bound = boundingRect(points);
    Mat mat;
    mat = Mat::zeros(bound.size(),CV_32S);
    int x0=bound.x;
    int y0=bound.y;
    for(int i=0;i<points.size();i++){
        mat.at<int>(   points[i].y -y0 , points[i].x -x0 )=1;
    }
    int s=  sum(mat)[0];
    std::stringstream ss;
    ss << s;
    return "D";
    return ss.str();
}


