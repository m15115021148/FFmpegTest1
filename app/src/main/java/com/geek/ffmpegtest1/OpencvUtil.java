package com.geek.ffmpegtest1;

import android.os.AsyncTask;

/**
 * Created by ${chenM} on 2018/11/28.
 */
public class OpencvUtil {

    static {
        System.loadLibrary("OpenCV");
    }

    public static void execute(String img1, String img2, String img3, String img4, final String path, final OpenCVRunListener listener){
        new AsyncTask<String, Double, Double>(){

            @Override
            protected void onPreExecute() {
                if (listener != null) {
                    listener.onStart();
                }
            }

            @Override
            protected Double doInBackground(String... params) {
                return merge(params[0],params[1],params[2],params[3],params[4]);
            }

            @Override
            protected void onPostExecute(Double integer) {
                if (listener != null) {
                    listener.onEnd(integer);
                }
            }
        }.execute(img1,img2,img3,img4,path);
    }


    public interface OpenCVRunListener{
        void onStart();
        void onEnd(double result);
    }

    public static native int[] gray(int[] pix,int width,int height);

    private static native double merge(String img1,String img2,String img3,String img4,String path);



}
