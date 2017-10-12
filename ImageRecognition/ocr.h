#ifndef OCR_H
#define OCR_H

#include "opencv2/opencv.hpp"

//Optical character regocniton

class OCR
{
public:
    OCR();
    std::string ocr(cv::Mat);

private:
    char character(cv::Mat);
    cv::vector<cv::Mat> split(cv::Mat);
    int fileNum;
    std::vector<cv::Mat> characters;
    void loadCharacters();
    std::vector<char> elements;

};

#endif // OCR_H
