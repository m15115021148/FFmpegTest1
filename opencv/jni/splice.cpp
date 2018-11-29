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
//#include <opencv2/stitching/stitcher.hpp>
//#include "custom_surf.h"

using namespace cv;
using namespace std;

#define LOG_TAG "Splice"
#define LOGD(...) ((void)__android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__))

#ifdef __cplusplus
extern "C" {
#endif

string jstring2str(JNIEnv* env, jstring jstr);

//计算原始图像点位在经过矩阵变换后在目标图像上对应位置
Point2f getTransformPoint(const Point2f originalPoint, const Mat &transformMaxtri);

JNIEXPORT jint JNICALL Java_com_geek_ffmpegtest1_OpencvUtil_split(JNIEnv *env, jclass clz, jstring img1, jstring img2,jstring img3){
/*
  //img_names.push_back(jstring2str(env,img1));
  //img_names.push_back(jstring2str(env,img2));

  //LOGD("open image1 %s" , img_names[0].c_str());
  //LOGD("open image2 %s" , img_names[1].c_str());


  //Mat image01 =  Mat(*(Mat*)img1);
  //cvtColor(image01, image01, CV_BGRA2BGR);
  //Mat image02  =  Mat(*(Mat*)img2);
  //cvtColor(image02, image02, CV_BGRA2BGR);

  Mat image01 = cv::imread(jstring2str(env,img1));
  cvtColor(image01, image01, CV_BGRA2BGR);
  Mat image02 = cv::imread(jstring2str(env,img2));
  cvtColor(image02, image02, CV_BGRA2BGR);

  if (image01.empty() || image02.empty())
  {
    //  printf("the loader the picture failed");
    //  waitKey();
      LOGD("there is some image input wrong");
      return 0;//图像没有全部读取成功
  }

  double time = getTickCount();

  LOGD("match start time=%f\n", time);

  //灰度图转换
  Mat image1, image2;
  cvtColor(image01, image1, CV_RGB2GRAY);
  cvtColor(image02, image2, CV_RGB2GRAY);

  //提取特征点
  //SiftFeatureDetector siftDetector(800);  // 海塞矩阵阈值
  Ptr<FeatureDetector> fastDetector = FastFeatureDetector::create();
  vector<KeyPoint> keyPoint1, keyPoint2;
  fastDetector->detect(image1, keyPoint1);
  fastDetector->detect(image2, keyPoint2);

  //特征点描述，为下边的特征点匹配做准备
  Ptr<DescriptorExtractor> BriskDescriptor = BRISK::create();
  Mat imageDesc1, imageDesc2;
  BriskDescriptor->compute(image1, keyPoint1, imageDesc1);
  BriskDescriptor->compute(image2, keyPoint2, imageDesc2);

  //获得匹配特征点，并提取最优配对
  Ptr<DescriptorMatcher> matcher = DescriptorMatcher::create("BruteForce");
  //FlannBasedMatcher matcher;
  vector<DMatch> matchePoints;
  matcher->match(imageDesc1, imageDesc2, matchePoints, Mat());
  if (matchePoints.size() < 10)
  {
      LOGD("the match point is below 10");
      //waitKey();
      return 0;
  }
  sort(matchePoints.begin(), matchePoints.end()); //特征点排序，opencv按照匹配点准确度排序
  //获取排在前N个的最优匹配特征点
  vector<Point2f> imagePoints1, imagePoints2;
  for (int i = 0; i<10; i++)
  {
      imagePoints1.push_back(keyPoint1[matchePoints[i].queryIdx].pt);
      imagePoints2.push_back(keyPoint2[matchePoints[i].trainIdx].pt);
  }

  //获取图像1到图像2的投影映射矩阵，尺寸为3*3
  Mat homo = findHomography(imagePoints1, imagePoints2, CV_RANSAC);
  Mat adjustMat;
  adjustMat = (Mat_<double>(3, 3) << 1.0, 0, image01.cols, 0, 1.0, 0, 0, 0, 1.0);//向后偏移image01.cols矩阵
  //Mat adjustMat =Mat::eye(cv::Size(3,3),CV_64F);
 // adjustMat.at<double>(0, 2) = image01.cols;
  Mat adjustHomo = adjustMat*homo;//矩阵相乘，先偏移

  //获取最强配对点（就是第一个配对点）在原始图像和矩阵变换后图像上的对应位置，用于图像拼接点的定位
  Point2f originalLinkPoint, targetLinkPoint, basedImagePoint;
  originalLinkPoint = keyPoint1[matchePoints[0].queryIdx].pt;
  targetLinkPoint = getTransformPoint(originalLinkPoint, adjustHomo);
  basedImagePoint = keyPoint2[matchePoints[0].trainIdx].pt;

  //图像配准
  Mat imageTransform;
  //将图片1进行映射到图像2，本来映射后x值为负值，但是把映射矩阵向后偏移image01.cols矩阵
  //我们很难判断出拼接后图像的大小尺寸，为了尽可能保留原来的像素，我们尽可能的大一些，对于拼接后的图片可以进一步剪切无效或者不规则的边缘
  warpPerspective(image01, imageTransform, adjustMat*homo, Size(image02.cols + image01.cols + 10, image02.rows));

  //在最强匹配点的位置处衔接，最强匹配点左侧是图1，右侧是图2，这样直接替换图像衔接不好，光线有突变
  //Mat ROIMat = image02(Rect(Point(basedImagePoint.x, 0), Point(image02.cols, image02.rows)));
  //ROIMat.copyTo(Mat(imageTransform1, Rect(targetLinkPoint.x, 0, image02.cols - basedImagePoint.x + 1, image02.rows)));

  //在最强匹配点左侧的重叠区域进行累加，是衔接稳定过渡，消除突变
  Mat image1Overlap, image2Overlap; //图1和图2的重叠部分
  image1Overlap = imageTransform(Rect(Point(targetLinkPoint.x - basedImagePoint.x, 0), Point(targetLinkPoint.x, image02.rows)));
  image2Overlap = image02(Rect(0, 0, image1Overlap.cols, image1Overlap.rows));
  Mat image1ROICopy = image1Overlap.clone();  //复制一份图1的重叠部分
  for (int i = 0; i<image1Overlap.rows; i++)
  {
      for (int j = 0; j<image1Overlap.cols; j++)
      {
          double weight;
          weight = (double)j / image1Overlap.cols;  //随距离改变而改变的叠加系数
          image1Overlap.at<Vec3b>(i, j)[0] = (1 - weight)*image1ROICopy.at<Vec3b>(i, j)[0] + weight*image2Overlap.at<Vec3b>(i, j)[0];
          image1Overlap.at<Vec3b>(i, j)[1] = (1 - weight)*image1ROICopy.at<Vec3b>(i, j)[1] + weight*image2Overlap.at<Vec3b>(i, j)[1];
          image1Overlap.at<Vec3b>(i, j)[2] = (1 - weight)*image1ROICopy.at<Vec3b>(i, j)[2] + weight*image2Overlap.at<Vec3b>(i, j)[2];
      }
  }
  Mat ROIMat = image02(Rect(Point(image1Overlap.cols, 0), Point(image02.cols, image02.rows)));  //图2中不重合的部分
  ROIMat.copyTo(Mat(imageTransform, Rect(targetLinkPoint.x, 0, ROIMat.cols, image02.rows))); //不重合的部分直接衔接上去

  time = getTickCount() - time;
  time /= getTickFrequency();
  LOGD("match time=%f\n", time);

  vector<int>compression_params;
  //compression_params.push_back(IMWRITE_PNG_COMPRESSION);
  compression_params.push_back(IMWRITE_JPEG_PROGRESSIVE);
 // compression_params.push_back(9);

  //Mat *ret = new Mat(imageTransform);

  imwrite(jstring2str(env,img3), imageTransform,compression_params);*/

  //int ret = splice(jstring2str(env,img1),jstring2str(env,img2),jstring2str(env,img3));

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

   imwrite(jstring2str(env,img3), pano,compression_params);

  return 0;
}

string jstring2str(JNIEnv* env, jstring jstr)
{   
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

//计算原始图像点位在经过矩阵变换后在目标图像上对应位置
Point2f getTransformPoint(const Point2f originalPoint, const Mat &transformMaxtri)
{
    Mat originelP, targetP;
    originelP = (Mat_<double>(3, 1) << originalPoint.x, originalPoint.y, 1.0);
    targetP = transformMaxtri*originelP;
    float x = targetP.at<double>(0, 0) / targetP.at<double>(2, 0);
    float y = targetP.at<double>(1, 0) / targetP.at<double>(2, 0);
    return Point2f(x, y);
}

#ifdef __cplusplus
}
#endif
