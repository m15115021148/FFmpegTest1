package com.geek.ffmpegtest1;

import android.app.Activity;
import android.app.ProgressDialog;
import android.content.Intent;
import android.net.Uri;
import android.os.Environment;
import android.support.annotation.Nullable;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.text.TextUtils;
import android.util.Log;
import android.view.View;
import android.widget.TextView;
import android.widget.Toast;

import com.geek.ffmpegtest1.util.FileNames;
import com.geek.ffmpegtest1.util.FileUtil;
import com.geek.ffmpegtest1.util.SystemFunUtil;

import java.io.File;
import java.io.IOException;
import java.util.ArrayList;
import java.util.List;

public class MainActivity extends AppCompatActivity {
    private String TAG = MainActivity.class.getSimpleName();
    private String base = Environment.getExternalStorageDirectory().getPath();
    private String savePath ;
    private String filePath = base + "/input.txt";
    private String outPath = base + "/merge.mp4";
    private SystemFunUtil util;
    private TextView mTv;
    private List<String> mList = new ArrayList<>();

    private ProgressDialog progressDialog;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        mTv = findViewById(R.id.content);

        util = new SystemFunUtil(this);
        mList.clear();

        progressDialog = new ProgressDialog(this);
        progressDialog.setCancelable(false);

    }

    public void onRun(View view) {

        if (FileUtil.isFileExist(outPath)){
            FileUtil.deleteFile(outPath);
        }

        if (mList.size()>1){
            savePath = FileUtil.getFolderName(mList.get(0));

            filePath = savePath + "/merge.txt";

            List<String> list = new ArrayList<>();

            for (int i=0;i<mList.size();i++){
                StringBuffer sb = new StringBuffer();
                sb.append("file")
                        .append(" ")
                        .append("'")
                        .append(FileUtil.getFileName(mList.get(i)))
                        .append("'");
                list.add(sb.toString());
            }

            boolean success = FileUtil.writeFile(filePath, list);

            mList.clear();

            if (success){
                FFmpegUtil.execute(CommandsUtil.mergeVideo(filePath, outPath), new FFmpegUtil.FFmpegRunListener() {
                    @Override
                    public void onStart() {
                        if (!progressDialog.isShowing())progressDialog.show();
                    }

                    @Override
                    public void onEnd(int result) {
                        if (progressDialog.isShowing())progressDialog.dismiss();
                        FileUtil.deleteFile(filePath);
                        if(result == 0){
                            Toast.makeText(MainActivity.this, "命令行执行完成", Toast.LENGTH_SHORT).show();
                        }
                    }
                });
            }

        }else {
            Toast.makeText(MainActivity.this, "两个以上", Toast.LENGTH_SHORT).show();
        }


    }

    public void onChoice(View view) {
        util.openCamera(SystemFunUtil.SYSTEM_IMAGE_PHONE,1);
    }

    @Override
    protected void onActivityResult(int requestCode, int resultCode, @Nullable Intent data) {
        if (resultCode== Activity.RESULT_OK){
            if (requestCode == 1){
                Uri uriVideo = data.getData();
                String path = util.getImageAbsolutePath(uriVideo);
                Log.d(TAG,"path-->"+path);
                if (TextUtils.isEmpty(path)){
                    return;
                }
                mList.add(path);
                showContent(path);
            }
        }

    }

    private void showContent(String path){
        StringBuffer sb = new StringBuffer();
        sb.append(FileUtil.getFileName(path)).append("\n");
        mTv.setText(String.format("%s%s", mTv.getText().toString(), sb.toString()));
    }

    public void onCommit(View view) {

        if (FileUtil.isFileExist(outPath)){
            FileUtil.deleteFile(outPath);
        }

        if (mList.size() == 2){

            FFmpegUtil.execute(CommandsUtil.multiVideo(mList.get(0), mList.get(1),outPath), new FFmpegUtil.FFmpegRunListener() {
                @Override
                public void onStart() {
                    if (!progressDialog.isShowing())progressDialog.show();
                }

                @Override
                public void onEnd(int result) {
                    if (progressDialog.isShowing())progressDialog.dismiss();
                    FileUtil.deleteFile(filePath);
                    if(result == 0){
                        Toast.makeText(MainActivity.this, "命令行执行完成", Toast.LENGTH_SHORT).show();
                    }
                }
            });

        }else {
            Toast.makeText(MainActivity.this, "只能两个", Toast.LENGTH_SHORT).show();
        }
    }

    public void onOpenCv(View view) {
        Intent intent = new Intent();
        intent.setClass(this,OpencvActivity.class);
        startActivity(intent);
    }

    public void onMergeVideo(View view) {
        Intent intent = new Intent();
        intent.setClass(this,MergeVideoActivity.class);
        startActivity(intent);
    }
}
