package com.geek.ffmpegtest1;

import android.util.Log;

import java.util.ArrayList;

/**
 * Created by ${chenM} on 2018/11/8.
 */
public class CommandsUtil {

    /**
     * txt文件拼接视频 时间上的拼接合并
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

    /**
     * 多画面拼接视频
     * @param input1 输入文件1
     * @param input2 输入文件2
     * @param targetFile 画面拼接文件
     *
     * @return 画面拼接的命令行
     */
    public static  String[] multiVideo(String input1, String input2, String targetFile){
        ArrayList<String> list = new ArrayList<>();
        list.add("ffmpeg");
        list.add("-i");
        list.add(input1);
        list.add("-i");
        list.add(input2);
        list.add("-filter_complex");
        list.add("hstack");//hstack:水平拼接，默认 ;vstack:垂直拼接
        list.add(targetFile);
        String[] commands = new String[list.size()];
        for (int i = 0; i < list.size(); i++) {
            commands[i] = list.get(i);
        }
        return commands;


    }


    /**
     * //ffmpeg -threads2 -y -r 10 -i /tmpdir/image%04d.jpg -i audio.mp3 -absf aac_adtstoasc output.mp4
     * @return
     *
     * ffmpeg -f image2 -i /images/%d.jpg -vcodec libx264 -r 25 -b 200k test.mp4
     */
    public static String[] playVideo(String inputPath,String outPathName){
        ArrayList<String> list = new ArrayList<>();
        list.add("ffmpeg");
        list.add("-f");
        list.add("image2");
        list.add("-i");
        list.add(inputPath);
        list.add("-vcodec");
        list.add("libx264");
        list.add("-r");
        list.add("25");
        list.add("-b");
        list.add("200k");
        list.add(outPathName);
        String[] commands = new String[list.size()];
        for (int i = 0; i < list.size(); i++) {
            commands[i] = list.get(i);
        }
        return commands;
    }




}
