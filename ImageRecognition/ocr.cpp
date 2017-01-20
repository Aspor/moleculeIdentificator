#include "ocr.h"
#include <QDebug>
#include<ctime>


using namespace cv;

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

    vector<Mat> chars = split(atom);
    std::string atomstr="";
    for(int i=0;i<chars.size();i++){
        //qDebug()<<"CharChar"<<i;
        atomstr+= character(chars[i]);
    }
    atomstr.erase(remove_if(atomstr.begin(), atomstr.end(), isspace), atomstr.end());


    qDebug()<<"atomSTR"<<QString::fromStdString( atomstr);
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
        //std::sort(atoms.begin(),atoms.end(), compPos );
    }
    return atoms;
}
//    for(int y=0;y<atom.rows;y++){
//        int* row = (int*) atom.ptr(y);
//        for(int x=0;x<atom.cols;x++){
//            if(row[x]!=0)
//                continue;

//            Rect rect;
//            Mat blob=Mat::zeros(atom.size(),CV_8UC3);
//            floodFill(atom,Point(x,y),1,&rect,0,0,8) ;


//            for (int i =rect.y;i<rect.y+rect.height;i++){
//                int* row2=(int*) atom.ptr(i);
//                for (int j =rect.x;j<rect.x+rect.width;j++){
////                    if(row2[j]=1)
////                        continue;

//                    blob.ptr(i)[j]=row2[j];
//                }
//            }
////            x=rect.x+rect.width;
////            y=rect.y+rect.height;
////            row = (int*) atom.ptr(y);




//            if(sum(blob)[0]>5)
//                countours.push_back(blob);
//        }
//    }
//    qDebug()<<"splited"<<countours.size();
//    return countours;
//}
char OCR::character(Mat c){
    int s=  sum(c)[0];
    qsrand(std::time(0) );
    fileNum=rand();
    //std::string fileName=std::to_string(fileNum);
    fileNum++;
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

   /*//    imwrite(fileName+".tif",c);
       Mat o;
        //return 'A';
        std::vector<double>bytes=std::vector<double>(120);
        resize(c,o,Size(12,10) );
        for(int i=0;i<120;i++){
            bytes[i]=o.data[i]/255.0;
        }
        return som->regognizeChar( bytes );
        //uchar* bits=o.data;
        return 'O';
        */
    }
    else
        return 'A';
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


