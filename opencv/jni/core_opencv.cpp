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

jdouble readVideo(JNIEnv *env, jclass obj,jstring video,jstring path){
    double time = getTickCount();

    LOGD("match start time=%f\n", time);

    string v_path = CUtil::jstringTostring(env,video);
    LOGD("v path play video %s",v_path.c_str());

    VideoCapture capture(v_path);

    if (!capture.isOpened()){
        LOGE("fail open video");
        return 0;
    }

    int id = 1;
    while (true) {
        LOGD("start -- read frame =%d",id);

        Mat frame;

        bool bSuccess = capture.read(frame);
        if (!bSuccess){
            LOGE("read mat frame is empty");
            break;
        }

       // capture >> frame;

        if (frame.empty()){
            LOGE("read mat frame is empty");
            break;
        }

        char name[512] = {0};
        sprintf(name, "%s/%0d.jpg", CUtil::jstringTostring(env,path).c_str(), id);

        imwrite(name, frame);

        id++;

    }
    capture.release();

    time = getTickCount() - time;
    time /= getTickFrequency();
    LOGD("match end time=%f\n", time);

    return time;
}

//---jni load--------
static const JNINativeMethod methodsRx[] = {
	{"gray", "([III)[I", (void*)gray },
	{"split", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)D", (void*)splice },
	{"readVideo","(Ljava/lang/String;Ljava/lang/String;)D",(void*)readVideo},
};

int register_core_opencv(JNIEnv *env){
	return jniRegisterNativeMethods(env, "com/geek/ffmpegtest1/OpencvUtil", methodsRx, NELEM(methodsRx) );
}

};

