package com.geek.ffmpegtest1;

import android.app.Activity;
import android.app.ProgressDialog;
import android.content.Intent;
import android.net.Uri;
import android.support.annotation.Nullable;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.text.TextUtils;
import android.util.Log;
import android.view.View;
import android.widget.TextView;
import android.widget.Toast;

import com.geek.ffmpegtest1.util.FileUtil;
import com.geek.ffmpegtest1.util.SystemFunUtil;

import java.util.ArrayList;
import java.util.List;

public class VideoActivity extends AppCompatActivity {
    private String TAG = VideoActivity.class.getSimpleName();
    private TextView mTv;
    private SystemFunUtil util;
    private List<String> mList = new ArrayList<>();
    private String savePath;
    private ProgressDialog progressDialog;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_video);

        mTv = findViewById(R.id.videoPath);

        util = new SystemFunUtil(this);

        progressDialog = new ProgressDialog(this);
        progressDialog.setCancelable(false);
    }

    public void onSelect(View view) {
        util.openCamera(SystemFunUtil.SYSTEM_IMAGE_PHONE,1);
//        util.openCamera(SystemFunUtil.SYSTEM_SELECT_IMAGE,1);
    }

    public void onRun(View view) {
        if (mList.size()>0){
            savePath = FileUtil.getFolderName(mList.get(0));

            OpencvUtil.readVideo(mList.get(0),savePath);



//        FFmpegUtil.execute(CommandsUtil.playVideo(savePath+"/%%d.jpg", savePath+"/video.mp4"), new FFmpegUtil.FFmpegRunListener() {
//            @Override
//            public void onStart() {
//                progressDialog.show();
//            }
//
//            @Override
//            public void onEnd(int result) {
//                progressDialog.dismiss();
//                Toast.makeText(VideoActivity.this,"success",Toast.LENGTH_SHORT).show();
//            }
//        });

        }
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
                mTv.setText(path);
            }
        }

    }
}
