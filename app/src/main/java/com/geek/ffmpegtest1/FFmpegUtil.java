package com.geek.ffmpegtest1;

import android.os.AsyncTask;
import android.util.Log;

import java.util.ArrayList;

/**
 * Created by ${chenM} on 2018/11/8.
 */
public class FFmpegUtil {

    static {
        System.loadLibrary("ffmpeg");
        System.loadLibrary("cubic");
    }


    public static void execute(String[] commands, final FFmpegRunListener listener){
        new AsyncTask<String[],Integer,Integer>(){

            @Override
            protected void onPreExecute() {
                if (listener != null) {
                    listener.onStart();
                }
            }

            @Override
            protected Integer doInBackground(String[]... params) {
                return run(params[0]);
            }

            @Override
            protected void onPostExecute(Integer integer) {
                if (listener != null) {
                    listener.onEnd(integer);
                }
            }
        }.execute(commands);
    }


    public interface FFmpegRunListener{
        void onStart();
        void onEnd(int result);
    }

    private static native int run(String[] commands);

}
