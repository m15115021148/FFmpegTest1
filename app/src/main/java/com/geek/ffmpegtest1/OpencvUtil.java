package com.geek.ffmpegtest1;

import android.os.AsyncTask;

/**
 * Created by ${chenM} on 2018/11/28.
 */
public class OpencvUtil {

    static {
        System.loadLibrary("OpenCV");
    }

    public static void execute(String img1,String img2,String path,final OpenCVRunListener listener){
        new AsyncTask<String, Double, Double>(){

            @Override
            protected void onPreExecute() {
                if (listener != null) {
                    listener.onStart();
                }
            }

            @Override
            protected Double doInBackground(String... params) {
                return split(params[0],params[1],params[2]);
            }

            @Override
            protected void onPostExecute(Double integer) {
                if (listener != null) {
                    listener.onEnd(integer);
                }
            }
        }.execute(img1,img2,path);
    }


    public interface OpenCVRunListener{
        void onStart();
        void onEnd(double result);
    }

    public static native int[] gray(int[] pix,int width,int height);

    private static native double split(String img1,String img2,String path);

    public static native double readVideo(String video,String path);

}
