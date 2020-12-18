#include "header.h"
Mat toGrayScale(Mat& image) {
    image.forEach<Vec3b>([](Vec3b& p, const int* pos) {
        int m = p[0] + p[1] + p[2];
        m /= 3;
        p[0] = m;
        p[1] = m;
        p[2] = m;

        });
    return image;
}

Mat angles;

Mat Sobel(Mat image) {
    vector<vector<double>> x(3);
    vector<vector<double>> y(3);
    x[0] = { -1.0, 0, 1.0 };
    x[1] = { -2.0, 0, 2.0 };
    x[2] = { -1.0, 0, 1.0 };
    y[0] = { 1.0, 2.0, 1.0 };
    y[1] = { 0, 0, 0 };
    y[2] = { -1.0, -2.0, -1.0 };

    Mat res = Mat(image.rows - 2, image.cols - 2, CV_8UC3);
    angles = Mat(image.rows - 2, image.cols - 2, CV_32FC3);
    for (int i = 1; i < image.rows - 1; ++i) {
        for (int j = 1; j < image.cols - 1; ++j) {
            double sumxR = 0;
            double sumxG = 0;
            double sumxB = 0;
            double sumyR = 0;
            double sumyG = 0;
            double sumyB = 0;
            for (int a = 0; a < x.size(); ++a) {
                for (int b = 0; b < x.size(); ++b) {
                    sumxB += x[a][b] * static_cast<double>((image.at<Vec3b>(i + a - 1, j + b - 1))[0]);
                    sumyB += y[a][b] * static_cast<double>((image.at<Vec3b>(i + a - 1, j + b - 1))[0]);
                    sumxG += x[a][b] * static_cast<double>((image.at<Vec3b>(i + a - 1, j + b - 1))[1]);
                    sumyG += y[a][b] * static_cast<double>((image.at<Vec3b>(i + a - 1, j + b - 1))[1]);
                    sumxR += x[a][b] * static_cast<double>((image.at<Vec3b>(i + a - 1, j + b - 1))[2]);
                    sumyR += y[a][b] * static_cast<double>((image.at<Vec3b>(i + a - 1, j + b - 1))[2]);
                }
            }
            Vec3b sqrt1(0, 0, 0);
            sqrt1[0] = sqrt(sumxB * sumxB + sumyB * sumyB);
            sqrt1[1] = sqrt(sumxG * sumxG + sumyG * sumyG);
            sqrt1[2] = sqrt(sumxR * sumxR + sumyR * sumyR);

            if (sqrt1[0] > 255 || sqrt1[1] > 255 || sqrt1[2] > 255) {
                sqrt1 = Vec3b(255, 255, 255);
            }
            res.at<Vec3b>(i - 1, j - 1) = sqrt1;

            if (sumxR == 0 || sumxG == 0 || sumxB == 0) {
                angles.at<Vec3f>(i - 1, j - 1) = Vec3f(90, 90, 90);
            }
            else {
                angles.at<Vec3f>(i - 1, j - 1) = Vec3f(atan(sumyB / sumxB), atan(sumyG / sumxG), atan(sumyR / sumxR));
            }
        }
    }
    return res;
}

Mat nonMax(Mat image) {
    Mat res = Mat(image.rows - 2, image.cols - 2, CV_8UC3);
    for (int i = 1; i < image.rows - 1; i++) {
        for (int j = 1; j < image.cols - 1; j++) {
            Vec3f t(0, 0, 0);
            t = angles.at<Vec3f>(i, j);
            res.at<Vec3b>(i - 1, j - 1) = image.at<Vec3b>(i - 1, j - 1);
            if ((((-22.5 < t[0]) && (t[0] <= 22.5)) || ((157.5 < t[0]) && (t[0] <= -157.5))) && (((-22.5 < t[1]) && (t[1] <= 22.5)) || ((157.5 < t[1]) && (t[1] <= -157.5))) && (((-22.5 < t[2]) && (t[2] <= 22.5)) || ((157.5 < t[2]) && (t[2] <= -157.5)))) {
                if (((image.at<Vec3b>(i, j)[0] < image.at<Vec3b>(i, j + 1)[0]) || (image.at<Vec3b>(i, j)[0] < image.at<Vec3b>(i, j - 1)[0])) && ((image.at<Vec3b>(i, j)[1] < image.at<Vec3b>(i, j + 1)[1]) || (image.at<Vec3b>(i, j)[1] < image.at<Vec3b>(i, j - 1)[1])) && ((image.at<Vec3b>(i, j)[2] < image.at<Vec3b>(i, j + 1)[2]) || (image.at<Vec3b>(i, j)[2] < image.at<Vec3b>(i, j - 1)[2]))) {
                    res.at<Vec3b>(i - 1, j - 1) = Vec3b(0, 0, 0);
                }
            }
            if ((((-112.5 < t[0]) && (t[0] <= -67.5)) || ((67.5 < t[0]) && (t[0] <= 112.5))) && (((-112.5 < t[1]) && (t[1] <= -67.5)) || ((67.5 < t[1]) && (t[1] <= 112.5))) && (((-112.5 < t[2]) && (t[2] <= -67.5)) || ((67.5 < t[2]) && (t[2] <= 112.5)))) {
                if (((image.at<Vec3b>(i, j)[0] < image.at<Vec3b>(i + 1, j)[0]) || (image.at<Vec3b>(i, j)[0] < image.at<Vec3b>(i - 1, j)[0])) && ((image.at<Vec3b>(i, j)[1] < image.at<Vec3b>(i + 1, j)[1]) || (image.at<Vec3b>(i, j)[1] < image.at<Vec3b>(i - 1, j)[1])) && ((image.at<Vec3b>(i, j)[2] < image.at<Vec3b>(i + 1, j)[2]) || (image.at<Vec3b>(i, j)[2] < image.at<Vec3b>(i - 1, j)[2]))) {
                    res.at<Vec3b>(i - 1, j - 1) = Vec3b(0, 0, 0);
                }
            }
            if ((((-67.5 < t[0]) && (t[0] <= -22.5)) || ((112.5 < t[0]) && (t[0] <= 157.5))) && (((-67.5 < t[1]) && (t[1] <= -22.5)) || ((112.5 < t[1]) && (t[1] <= 157.5))) && (((-67.5 < t[2]) && (t[2] <= -22.5)) || ((112.5 < t[2]) && (t[2] <= 157.5)))) {
                if (((image.at<Vec3b>(i, j)[0] < image.at<Vec3b>(i - 1, j + 1)[0]) || (image.at<Vec3b>(i, j)[0] < image.at<Vec3b>(i + 1, j - 1)[0])) && ((image.at<Vec3b>(i, j)[1] < image.at<Vec3b>(i - 1, j + 1)[1]) || (image.at<Vec3b>(i, j)[1] < image.at<Vec3b>(i + 1, j - 1)[1])) && ((image.at<Vec3b>(i, j)[2] < image.at<Vec3b>(i - 1, j + 1)[2]) || (image.at<Vec3b>(i, j)[2] < image.at<Vec3b>(i + 1, j - 1)[2]))) {
                    res.at<Vec3b>(i - 1, j - 1) = Vec3b(0, 0, 0);
                }
            }
            if ((((-157.5 < t[0]) && (t[0] <= -112.5)) || ((22.5 < t[0]) && (t[0] <= 67.5))) && (((-157.5 < t[1]) && (t[1] <= -112.5)) || ((22.5 < t[1]) && (t[1] <= 67.5))) && (((-157.5 < t[2]) && (t[2] <= -112.5)) || ((22.5 < t[2]) && (t[2] <= 67.5)))) {
                if (((image.at<Vec3b>(i, j)[0] < image.at<Vec3b>(i + 1, j + 1)[0]) || (image.at<Vec3b>(i, j)[0] < image.at<Vec3b>(i - 1, j - 1)[0])) && ((image.at<Vec3b>(i, j)[1] < image.at<Vec3b>(i + 1, j + 1)[1]) || (image.at<Vec3b>(i, j)[1] < image.at<Vec3b>(i - 1, j - 1)[1])) && ((image.at<Vec3b>(i, j)[2] < image.at<Vec3b>(i + 1, j + 1)[2]) || (image.at<Vec3b>(i, j)[2] < image.at<Vec3b>(i - 1, j - 1)[2]))) {
                    res.at<Vec3b>(i - 1, j - 1) = Vec3b(0, 0, 0);
                }
            }
        }
    }
    return res;
}

Mat threshold(Mat image, int a, int b) {
    if (a > 255) {
        a = 255;
    }
    if (b > 255) {
        b = 255;
    }
    Mat edge = Mat(image.rows, image.cols, CV_8UC3);
    for (int i = 0; i < image.rows; ++i) {
        for (int j = 0; j < image.cols; ++j) {
            edge.at<Vec3b>(i, j) = image.at<Vec3b>(i, j);
            if (edge.at<Vec3b>(i, j)[0] > b && edge.at<Vec3b>(i, j)[1] > b && edge.at<Vec3b>(i, j)[2] > b) {
                edge.at<Vec3b>(i, j) = Vec3b(255, 255, 255);
            }
            else if (edge.at<Vec3b>(i, j)[0] < a && edge.at<Vec3b>(i, j)[1] < a && edge.at<Vec3b>(i, j)[2] < a) {
                edge.at<Vec3b>(i, j) = Vec3b(0, 0, 0);
            }
            else {
                int H = 0, calc = 0;
                for (int x = i - 1; x <= i + 1; ++x) {
                    for (int y = j - 1; y <= j + 1; ++y) {
                        if (x <= 0 || y <= 0 || edge.rows || y > edge.cols) continue;
                        else {
                            if (edge.at<Vec3b>(x, y)[0] > b && edge.at<Vec3b>(x, y)[1] > b && edge.at<Vec3b>(x, y)[2] > b) {
                                edge.at<Vec3b>(i, j) = Vec3b(255, 255, 255);
                                H = 1;
                                break;
                            }
                            else if (edge.at<Vec3b>(x, y)[0] <= b && edge.at<Vec3b>(x, y)[0] >= a && edge.at<Vec3b>(x, y)[1] <= b && edge.at<Vec3b>(x, y)[1] >= a && edge.at<Vec3b>(x, y)[2] <= b && edge.at<Vec3b>(x, y)[2] >= a) {
                                calc = 1;
                            }
                        }
                    }
                    if (H) break;
                }
                if (!H && calc) {
                    for (int x = i - 2; x <= i + 2; ++x) {
                        for (int y = j - 2; y <= j + 2; ++y) {
                            if (x < 0 || y < 0 || x > edge.rows || y > edge.cols) continue;
                            else {
                                if (edge.at<Vec3b>(x, y)[0] > b && edge.at<Vec3b>(x, y)[1] > b && edge.at<Vec3b>(x, y)[2] > b) {
                                    edge.at<Vec3b>(i, j) = Vec3b(255, 255, 255);
                                    H = 1;
                                    break;
                                }
                            }
                        }
                        if (H) break;
                    }
                }
                if (!H) {
                    edge.at<Vec3b>(i, j) = Vec3b(0, 0, 0);
                }
            }
        }
    }
    return edge;
}

template <class T> T check(T v, int max, int min) {
    if (v > max) return max;
    else if (v < min) return min;
    else return v;
}

Mat Gauss(Mat& in, int radius, float sigma) {
    Mat res(in);
    int size = 2 * radius + 1;
    float normir = 0;
    vector<float>vec(size * size);
    for (int i = -radius; i <= radius; i++) {
        for (int j = -radius; j <= radius; j++) {
            int idx = (i + radius) * size + (j + radius);
            vec[idx] = exp(-(i * i + j * j) / (2 * sigma * sigma));
            vec[idx] /= (2 * 3.14 * sigma * sigma);
            normir += vec[idx];
        }
    }
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            int n = i * size + j;
            vec[n] /= normir;
        }
    }
    for (int x = 0; x < in.rows; ++x) {
        for (int y = 0; y < in.cols; ++y) {
            Vec3b tmp(0, 0, 0);
            for (int i = -radius; i <= radius; i++) {
                for (int j = -radius; j <= radius; j++) {
                    int idx = (i + radius) * size + j + radius;
                    tmp[0] += res.at<Vec3b>(check<int>(x + i, in.rows - 1, 0), check<int>(y + j, in.cols - 1, 0))[0] * vec[idx];
                    tmp[1] += res.at<Vec3b>(check<int>(x + i, in.rows - 1, 0), check<int>(y + j, in.cols - 1, 0))[1] * vec[idx];
                    tmp[2] += res.at<Vec3b>(check<int>(x + i, in.rows - 1, 0), check<int>(y + j, in.cols - 1, 0))[2] * vec[idx];
                }
            }
            res.at<Vec3b>(x, y) = Vec3b(check(tmp[0], 255, 0), check(tmp[1], 255, 0), check(tmp[2], 255, 0));
        }
    }
    return res;
}