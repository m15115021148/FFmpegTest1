//
// Created by cmm on 2018/11/30.
//
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>

#include "cubic_inc.h"

using namespace std;

class CUtil{
public:

    static string jstringTostring(JNIEnv *env,jstring jstr){
        char* rtn = NULL;
        jclass clsstring = env->FindClass("java/lang/String");
        jstring strencode = env->NewStringUTF("GB2312");
        jmethodID mid = env->GetMethodID(clsstring, "getBytes", "(Ljava/lang/String;)[B");
        jbyteArray barr = (jbyteArray)env->CallObjectMethod(jstr,mid,strencode);
        jsize alen = env->GetArrayLength(barr);
        jbyte* ba = env->GetByteArrayElements(barr,JNI_FALSE);
        if(alen > 0){
            rtn = (char*)malloc(alen+1);
            memcpy(rtn,ba,alen);
            rtn[alen]=0;
        }
        env->ReleaseByteArrayElements(barr,ba,0);
        std::string stemp(rtn);
        free(rtn);
        return stemp;
    };

};
