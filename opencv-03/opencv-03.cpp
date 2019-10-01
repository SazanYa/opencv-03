#include "pch.h"
#include "opencv2/opencv.hpp"

using namespace cv;

// детектор ребер Канни
int applyCanny(Mat& src, Mat& dst)
{
    // пороговые значения для отсечения
    double threshold1 = 70, threshold2 = 260;
    cvtColor(src, dst, 7);
    GaussianBlur(dst, dst, Size(7, 7), 1.5, 1.5);
    Canny(dst, dst, threshold1, threshold2);
    return 0;
}

// оператор Собеля
int applySobel(Mat& src, Mat& dst)
{
    int ddepth = CV_16S;
    double alpha = 0.5, beta = 0.5;

    Mat grayImg, xGrad, yGrad,
        xGradAbs, yGradAbs;

    GaussianBlur(src, src, Size(3, 3),
        0, 0, BORDER_DEFAULT);
    // преобразование в оттенки серого
    cvtColor(src, grayImg, 7);
    // вычисление производных по двум направлениям
    Sobel(grayImg, xGrad, ddepth, 1, 0); // по Ox
    Sobel(grayImg, yGrad, ddepth, 0, 1); // по Oy
    // преобразование градиентов в 8-битные
    convertScaleAbs(xGrad, xGradAbs);
    convertScaleAbs(yGrad, yGradAbs);
    // поэлементное вычисление взвешенной суммы двух массивов
    addWeighted(xGradAbs, alpha, yGradAbs, beta, 0, dst);
    return 0;
}

// оператор Лапласа
int applyLaplacian(Mat& src, Mat& dst)
{
    Mat grayImg, laplacianImg;
    int ddepth = CV_16S;

    GaussianBlur(src, src, Size(3, 3),
        0, 0, BORDER_DEFAULT);
    // преобразование в оттенки серого
    cvtColor(src, grayImg, 7);
    // применение оператора Лапласа
    Laplacian(grayImg, laplacianImg, ddepth);
    convertScaleAbs(laplacianImg, dst);
    return 0;
}

int main(int, char**)
{
    // открыть камеру по умолчанию
    VideoCapture cap(0);
    if (!cap.isOpened())
        return -1;

    Mat edges;
    namedWindow("edges", 1);
    for (;;)
    {
        Mat frame;
        // получить новый кадр из камеры
        cap >> frame;
        //applySobel(frame, edges);
        //applyLaplacian(frame, edges);
        applyCanny(frame, edges);
        imshow("edges", edges);
        if (waitKey(30) >= 0) break;
    }

    return 0;
}
