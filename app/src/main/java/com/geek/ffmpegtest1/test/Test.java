package com.geek.ffmpegtest1.test;

/**
 * cmmdeMacBook-Pro:java cmm$ javah -d ../../../jni -jni com.geek.ffmpegtest1.test.Test
 * Created by ${chenM} on 2018/11/30.
 */
public class Test {
    public static native int[] gray(int[] pix,int width,int height);

    private static native double split(String img1,String img2,String path);

    private static native int run(String[] commands);
}
