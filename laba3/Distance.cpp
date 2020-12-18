#include "header.h"


void DistanceTransform(InputArray _src, OutputArray _dst, int distType, int maskSize) {
    Mat src = _src.getMat();
    const int a = 1.0f * 99999;
    const int b = 1.4f * 99999;
    const int c = 2.1969f * 99999;
    Size srcsize = src.size();
    Mat temp(srcsize.height + 2 * 2, srcsize.width + 2 * 2, CV_32SC1);
    int* ttemp = temp.ptr<int>();
    int tempstep = static_cast<int>(temp.step / sizeof(ttemp[0]));
    for (int i = 0; i < 2; ++i) {
        int* rowi = temp.ptr<int>(i);
        for (int j = 0; j < temp.size().width; ++j) {
            rowi[j] = 0;
        }
    }
    for (int i = 0; i < 2; ++i) {
        int* rowi = temp.ptr<int>(temp.size().height - i - 1);
        for (int j = 0; j < temp.size().width; ++j) {
            rowi[j] = 0;
        }
    }
    uchar* ssrc = src.ptr(); 
    int srcstep = static_cast<int>(src.step / sizeof(ssrc[0]));
    for (int i = 0; i < _src.size().height; i++) {
        const uchar* curr = ssrc + i * srcstep;
        int* tmp = static_cast<int*>(ttemp + (i + 2) * tempstep) + 2;
        for (int j = 0; j < _src.size().width; j++) {
            if (curr[j] == 0) {
                tmp[j] = 0;
            }
            else {
                tmp[j] = min({ tmp[j - tempstep * 2 - 1] + c ,
                    tmp[j - tempstep * 2 + 1] + c ,
                    tmp[j - tempstep - 2] + c ,
                    tmp[j - tempstep - 1] + b ,
                    tmp[j - tempstep] + a ,
                    tmp[j - tempstep + 1] + b ,
                    tmp[j - tempstep + 2] + c ,
                    tmp[j - 1] + a });
            }
        }
    }
    _dst.create(src.size(), CV_32F);
    Mat dst = _dst.getMat();
    float* ddist = dst.ptr<float>();
    int dststep = static_cast<int>(dst.step / sizeof(ddist[0]));
    for (int i = _src.size().height - 1; i >= 0; --i) {
        float* distance = static_cast<float*>(ddist + i * dststep);
        int* tmp = static_cast<int*>(ttemp + (i + 2) * tempstep) + 2;
        for (int j = _src.size().width - 1; j >= 0; --j) {
            int t0 = tmp[j];
            if (t0 > a) {
                t0 = min({ tmp[j], tmp[j + tempstep * 2 + 1] + c,
                        tmp[j + tempstep * 2 - 1] + c,
                        tmp[j + tempstep - 2] + c,
                        tmp[j + tempstep + 2] + c,
                        tmp[j + tempstep + 1] + b,
                        tmp[j + tempstep] + a,
                        tmp[j + tempstep - 1] + b,
                        tmp[j + 1] + a });
                tmp[j] = t0;
            }
            distance[j] = static_cast<float>(t0 * 1.f / (100000));
        }
    }
}