package com.geek.ffmpegtest1;

import android.os.AsyncTask;

/**
 * Created by ${chenM} on 2018/11/28.
 */
public class OpencvUtil {

    static {
//        System.loadLibrary("opencv_java3");
        System.loadLibrary("OpenCV");
    }

//    public static void execute(int[] pix, int width,int height,final OpenCVRunListener listener){
//        new AsyncTask<String[],Integer,Integer>(){
//
//            @Override
//            protected void onPreExecute() {
//                if (listener != null) {
//                    listener.onStart();
//                }
//            }
//
//            @Override
//            protected Integer doInBackground(String[]... params) {
//                return run(params[0]);
//            }
//
//            @Override
//            protected void onPostExecute(Integer integer) {
//                if (listener != null) {
//                    listener.onEnd(integer);
//                }
//            }
//        }.execute(commands);
//    }


    public interface OpenCVRunListener{
        void onStart();
        void onEnd(int result);
    }

    public static native int[] gray(int[] pix,int width,int height);

    public static native int split(String img1,String img2,String path);

}
