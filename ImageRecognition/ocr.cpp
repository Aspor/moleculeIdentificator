#include "ocr.h"
#include <QDebug>
#include<ctime>


using namespace cv;

//Compare position of two rectangles so elements
//are read from left to right and top to bottom.

bool compPos(vector<Point> a,vector<Point> b  ){
    qDebug()<<"compPOS";
    Rect A = boundingRect(a);
    Rect B = boundingRect(b);

    int xDist =abs( A.x-B.x);
    int yDist = abs(A.y-B.y);
    if(xDist>yDist){
        return A.x<B.x;
    }
    else
        return A.y<B.y;
}

OCR::OCR()
{
    loadCharacters();
}

std::string OCR::ocr(Mat atom){

    //Split elements to single characters
    vector<Mat> chars = split(atom);
    std::string atomstr="";

    //Regocnice characters and append them to atom string
    for(int i=0;i<chars.size();i++){
        //qDebug()<<"CharChar"<<i;
        atomstr+= character(chars[i]);
    }

    //Remove spaces from string
    atomstr.erase(remove_if(atomstr.begin(), atomstr.end(), isspace), atomstr.end());

    if(atomstr.length()==0)
        return "C";
    return atomstr;
}

vector<Mat> OCR::split(Mat atom){
    vector<Mat> atoms;
    Mat img;
    cvtColor( atom, img, COLOR_BGR2GRAY );
    bitwise_not(img,img);
    blur(img,img,Size(2,2));

//    namedWindow("atoms");
//    imshow("atoms",atom);
//    waitKey(0);

    vector<vector<Point> >contours;
    findContours(img,contours, CV_RETR_LIST, CHAIN_APPROX_NONE, Point(0,0) );

    std::sort(contours.begin(),contours.end(),compPos);

    for(int i=0;i<contours.size();i++){
        Rect rect = boundingRect(contours[i]);
        Mat mat(atom,rect);
        qDebug()<<"i"<<i;
        if(sum(mat)[0]>500&&rect.area()<500 && rect.width/rect.height<10 && rect.height/rect.width<10 ) {
            qDebug()<<rect.area();
            atoms.push_back(mat);
        }
    }
    return atoms;
}

char OCR::character(Mat c){
    int s=  sum(c)[0];
    qsrand(std::time(0) );
   // fileNum=rand();
    //std::string fileName=std::to_string(fileNum);
   // fileNum++;
    if(s>1000){
        Mat o;
        resize(c,o,Size(12,10));
        char nearest;
        int shortest=-1;
        cvtColor(o,o,COLOR_BGR2GRAY);
        for(int i=0;i<characters.size();i++){
            int dist=sum(o!=characters[i])[0];
            //qDebug()<<dist<<shortest<<"dist";
            if(dist<shortest ||  shortest==-1){
                nearest=elements[i];
                shortest=dist;
            }
        }
        if(shortest<15000)
            return nearest;
        return ' ';
    }
    else
        return ' ';
}

void OCR::loadCharacters(){
    characters.push_back( imread("../moleculeIdentificator/ocrData/C87755422.tif" ,IMREAD_GRAYSCALE));
    characters.push_back( imread("../moleculeIdentificator/ocrData/F1102520059.tif" ,IMREAD_GRAYSCALE));
    characters.push_back( imread("../moleculeIdentificator/ocrData/H1139901474.tif" ,IMREAD_GRAYSCALE));
    characters.push_back( imread("../moleculeIdentificator/ocrData/N1100661313.tif" ,IMREAD_GRAYSCALE));
    characters.push_back( imread("../moleculeIdentificator/ocrData/O439493451.tif" ,IMREAD_GRAYSCALE));
    characters.push_back( imread("../moleculeIdentificator/ocrData/S749241873.tif" ,IMREAD_GRAYSCALE));
    characters.push_back( imread("../moleculeIdentificator/ocrData/e846930886.tif" ,IMREAD_GRAYSCALE));
    characters.push_back( imread("../moleculeIdentificator/ocrData/M1681692777.tif" ,IMREAD_GRAYSCALE));

    elements={'C','F','H','N','O','S','e','M'};
}


