//
// Created by cmm on 2018/11/30.
//
#include <jni.h>
#include "cubic_inc.h"
#include <stdio.h>

#include "ffmpeg/ffmpeg.h"

JNIEXPORT jint JNICALL Java_com_geek_ffmpegtest1_FFmpegUtil_run(JNIEnv *env, jclass obj,jobjectArray commands){
    int argc = (*env)->GetArrayLength(env, commands);
    char *argv[argc];

    int i;
    for (i = 0; i < argc; i++) {
        jstring js = (jstring) (*env)->GetObjectArrayElement(env, commands, i);
        argv[i] = (char*) (*env)->GetStringUTFChars(env, js, 0);
    }
    return run(argc, argv);
}