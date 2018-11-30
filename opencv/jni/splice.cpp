#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <jni.h>
#include <android/log.h>
#include <iostream>
#include <fstream>

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/calib3d/calib3d.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/features2d/features2d.hpp"
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

#define LOG_TAG "Splice"
#define LOGD(...) ((void)__android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__))

#ifdef __cplusplus
extern "C" {
#endif

string jstring2str(JNIEnv* env, jstring jstr);

JNIEXPORT jdouble JNICALL Java_com_geek_ffmpegtest1_OpencvUtil_split(JNIEnv *env, jclass clz, jstring img1, jstring img2,jstring img3){

    double time = getTickCount();

    LOGD("match start time=%f\n", time);

    Mat pano;

    Stitcher stitcher = Stitcher::createDefault(false);

    vector<Mat>img_names;

    Mat image01 = cv::imread(jstring2str(env,img1));
    cvtColor(image01, image01, CV_BGRA2BGR);

    Mat image02 = cv::imread(jstring2str(env,img2));
    cvtColor(image02, image02, CV_BGRA2BGR);

    img_names.push_back(image01);
    img_names.push_back(image02);

    vector<int>compression_params;
    compression_params.push_back(IMWRITE_JPEG_PROGRESSIVE);

    Stitcher::Status status = stitcher.stitch(img_names, pano);

    time = getTickCount() - time;
    time /= getTickFrequency();
    LOGD("match time=%f\n", time);

    imwrite(jstring2str(env,img3), pano,compression_params);

  return time;
}

string jstring2str(JNIEnv* env, jstring jstr){
    char*   rtn   =   NULL;   
    jclass   clsstring   =   env->FindClass("java/lang/String");   
    jstring   strencode   =   env->NewStringUTF("GB2312");   
    jmethodID   mid   =   env->GetMethodID(clsstring,   "getBytes",   "(Ljava/lang/String;)[B");   
    jbyteArray   barr=   (jbyteArray)env->CallObjectMethod(jstr,mid,strencode);   
    jsize   alen   =   env->GetArrayLength(barr);   
    jbyte*   ba   =   env->GetByteArrayElements(barr,JNI_FALSE);   
    if(alen   >   0)   
    {   
        rtn   =   (char*)malloc(alen+1);         
        memcpy(rtn,ba,alen);   
        rtn[alen]=0;   
    }   
    env->ReleaseByteArrayElements(barr,ba,0);   
    std::string stemp(rtn);
    free(rtn);
    return   stemp;   
}

#ifdef __cplusplus
}
#endif
