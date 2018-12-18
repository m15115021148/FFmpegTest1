package com.geek.ffmpegtest1;

import android.app.ProgressDialog;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.Toast;

public class MergeVideoActivity extends AppCompatActivity {
    private String imgPath = "/storage/emulated/0/DCIM/Camera/image/merge/";
    private String videoName = "storage/emulated/0/DCIM/Camera/opencv/play_video.avi";
    private ProgressDialog progressDialog;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_merge_video);

        progressDialog = new ProgressDialog(this);
        progressDialog.setCancelable(false);
    }

    public void onMergeRun(View view) {
        OpencvUtil.executeVideo(imgPath, videoName, new OpencvUtil.OpenCVRunListener() {
            @Override
            public void onStart() {
                progressDialog.show();
            }

            @Override
            public void onEnd(double result) {
                progressDialog.dismiss();
                Toast.makeText(MergeVideoActivity.this,"success",Toast.LENGTH_SHORT).show();
            }
        });
    }
}
