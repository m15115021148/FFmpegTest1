//
// Created by cmm on 2018/11/30.
//
#include "JNIHelp.h"
#include "cubic_inc.h"
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>

#include <sys/stat.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>

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

namespace android {

jintArray gray(JNIEnv *env, jclass obj, jintArray buf, int w, int h){
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

jdouble splice(JNIEnv *env, jclass obj,jstring img1,jstring img2,jstring img3){

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

jdouble readVideo(JNIEnv *env, jclass obj,jstring videoFirst, jstring videoSecond, jstring path){
    double time = getTickCount();

    string v_first = CUtil::jstringTostring(env,videoFirst);
    string v_second = CUtil::jstringTostring(env,videoSecond);

    VideoCapture capture1(v_first);
    VideoCapture capture2(v_second);

    if (!capture1.isOpened() && !capture2.isOpened() ){
        LOGE("fail open video");
        return -1;
    }

    int id = 1;

    while (true) {
        LOGD("start -- read frame =%d",id);

        Mat frame1,frame2;

        bool b_first = capture1.read(frame1);
        bool b_second = capture2.read(frame2);

        if (!b_first && !b_second ){
            LOGE("read mat frame is empty");
            break;
        }

        if (frame1.empty() || frame2.empty() ){
            LOGE("read mat frame is empty");
            break;
        }

        char name1[512] = {0};
        char name2[521] = {0};
        sprintf(name1, "%s/first/%0d.jpg", CUtil::jstringTostring(env,path).c_str(), id);
        sprintf(name2, "%s/second/%0d.jpg", CUtil::jstringTostring(env,path).c_str(), id);

        imwrite(name1, frame1);
        imwrite(name2, frame2);

        id++;

    }
    capture1.release();
    capture2.release();

    time = getTickCount() - time;
    time /= getTickFrequency();
    LOGD("match end time=%f\n", time);

    return time;
}

//---jni load--------
static const JNINativeMethod methodsRx[] = {
	{"gray", "([III)[I", (void*)gray },
	{"split", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)D", (void*)splice },
	{"readVideo","(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)D",(void*)readVideo},
};

int register_core_opencv(JNIEnv *env){
	return jniRegisterNativeMethods(env, "com/geek/ffmpegtest1/OpencvUtil", methodsRx, NELEM(methodsRx) );
}

};

