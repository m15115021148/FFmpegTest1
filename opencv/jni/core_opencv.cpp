//
// Created by cmm on 2018/11/30.
//
#include <jni.h>
#include "cubic_inc.h"
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>

#include "CUtil.cpp"

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/calib3d/calib3d.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/features2d/features2d.hpp"
#include <opencv2/opencv.hpp>

#ifdef CUBIC_LOG_TAG
#undef CUBIC_LOG_TAG
#endif //CUBIC_LOG_TAG
#define CUBIC_LOG_TAG  "OpenCV"

using namespace cv;
using namespace std;


JNIEXPORT jintArray JNICALL Java_com_geek_ffmpegtest1_OpencvUtil_gray(JNIEnv *env, jclass obj, jintArray buf, int w, int h){
	jint *cbuf;
    cbuf = env->GetIntArrayElements(buf, JNI_FALSE );
    if (cbuf == NULL) {
      return 0;
    }

    Mat imgData(h, w, CV_8UC4, (unsigned char *) cbuf);

    uchar* ptr = imgData.ptr(0);
    for(int i = 0; i < w*h; i ++){
      //计算公式：Y(亮度) = 0.299*R + 0.587*G + 0.114*B
      //对于一个int四字节，其彩色值存储方式为：BGRA
      int grayScale = (int)(ptr[4*i+2]*0.299 + ptr[4*i+1]*0.587 + ptr[4*i+0]*0.114);
      ptr[4*i+1] = grayScale;
      ptr[4*i+2] = grayScale;
      ptr[4*i+0] = grayScale;
    }

    int size = w * h;
    jintArray result = env->NewIntArray(size);
    env->SetIntArrayRegion(result, 0, size, cbuf);
    env->ReleaseIntArrayElements(buf, cbuf, 0);
    return result;
}

JNIEXPORT jdouble JNICALL Java_com_geek_ffmpegtest1_OpencvUtil_splice(JNIEnv *env, jclass obj,jstring img1,jstring img2,jstring img3){

    double time = getTickCount();

    LOGD("match start time=%f\n", time);

    Mat pano;

    Stitcher stitcher = Stitcher::createDefault(false);

    vector<Mat>img_names;

    Mat image01 = cv::imread(CUtil::jstringTostring(env,img1));
    cvtColor(image01, image01, CV_BGRA2BGR);

    Mat image02 = cv::imread(CUtil::jstringTostring(env,img2));
    cvtColor(image02, image02, CV_BGRA2BGR);

    img_names.push_back(image01);
    img_names.push_back(image02);

    vector<int>compression_params;
    compression_params.push_back(IMWRITE_JPEG_PROGRESSIVE);

    Stitcher::Status status = stitcher.stitch(img_names, pano);

    time = getTickCount() - time;
    time /= getTickFrequency();
    LOGD("match time=%f\n", time);

    imwrite(CUtil::jstringTostring(env,img3), pano,compression_params);

    return time;
}

