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

extern "C" {
#include "ffmpeg/ffmpeg.h"
};


namespace android {

extern "C"
jint run(JNIEnv *env, jclass obj,jobjectArray commands){
    int argc = env->GetArrayLength(commands);
    char *argv[argc];

    int i;
    for (i = 0; i < argc; i++) {
        jstring js = (jstring) env->GetObjectArrayElement(commands, i);
        argv[i] = (char*) env->GetStringUTFChars(js, 0);
    }
    return run(argc, argv);
}


//---jni load--------
static const JNINativeMethod methodsRx[] = {
	{"run", "([Ljava/lang/String;)I", (void*)run },
};

int register_core_ffmpeg(JNIEnv *env){
	return jniRegisterNativeMethods(env, "com/geek/ffmpegtest1/FFmpegUtil", methodsRx, NELEM(methodsRx) );
}

};
