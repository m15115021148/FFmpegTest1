package com.geek.ffmpegtest1;

import android.util.Log;

import java.util.ArrayList;

/**
 * Created by ${chenM} on 2018/11/8.
 */
public class CommandsUtil {

    /**
     * txt文件拼接视频
     * eg:ffmpeg -f concat -i list.txt -c copy concat.mp4
     * list.txt
     *  file 'input1.mp4'
     *  file 'input2.mp4'
     */
    public static String[] mergeVideo(String filePath, String outPath) {
        Log.w("jack","filePath:" + filePath + "\noutPath:" + outPath);
        ArrayList<String> list = new ArrayList<>();
        list.add("ffmpeg");
        list.add("-f");
        list.add("concat");
        list.add("-safe");
        list.add("0");
        list.add("-i");
        list.add(filePath);
        list.add("-c");
        list.add("copy");
        list.add(outPath);
        String[] commands = new String[list.size()];
        for (int i = 0; i < list.size(); i++) {
            commands[i] = list.get(i);
        }
        return commands;
    }


}
