#include "header.h"

int main() {
    String image_name1("../image2.jpg");
    Mat image1 = imread(image_name1);
    imshow("Original", image1);
    waitKey();

    Mat imageCanny = image1;
    Mat step1 = Mat(toGrayScale(imageCanny));
    Mat step2 = Mat(Gauss(step1, 3, 0.02));
    Mat step3 = Mat(Sobel(step2));
    Mat step4 = Mat(nonMax(step3));
    Mat step5 = Mat(threshold(step4, 20, 40));
    imshow("Canny - Task1", step5);
    waitKey();

    Mat imageCannyOCV = image1;
    cvtColor(imageCannyOCV, imageCannyOCV, COLOR_BGR2GRAY);
    Canny(imageCannyOCV, imageCannyOCV, 20, 40);
    imshow("OpenCV_Canny - Task4", imageCannyOCV);
    waitKey();

    String image_name2("../image3.jpg");
    Mat imageDist = imread(image_name2);
    imshow("Original", imageDist);
    cvtColor(imageDist, imageDist, COLOR_BGR2GRAY);
    threshold(imageDist, imageDist, 0, 255, THRESH_BINARY_INV + THRESH_OTSU);
    dilate(imageDist, imageDist, Mat::ones(3, 3, CV_8U));
    Mat dist;
    DistanceTransform(imageDist, dist, DIST_L2, 5);
    normalize(dist, dist, 0, 1.0, NORM_MINMAX);
    imshow("Distance - Task2", dist);
    waitKey();

    String image_name3("../image4.jpg");
    Mat imageWS = imread(image_name3);
    imshow("Original", imageWS);
    Mat water = watershed(imageWS);
    imshow("WaterShed - Task3", water);
    waitKey();

    return 0;
}
