package com.geek.ffmpegtest1;

import android.app.Activity;
import android.content.Intent;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.net.Uri;
import android.support.annotation.Nullable;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.text.TextUtils;
import android.util.Log;
import android.view.View;
import android.widget.ImageView;
import android.widget.TextView;

import com.geek.ffmpegtest1.util.SystemFunUtil;

public class OpencvActivity extends AppCompatActivity {
    private String TAG = OpencvActivity.class.getSimpleName();
    private TextView mSelect,mRun;
    private ImageView img1,img2;
    private SystemFunUtil util;
    private Bitmap bitmap;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_opencv);
        mSelect = findViewById(R.id.select);
        mRun = findViewById(R.id.run);
        img1 = findViewById(R.id.img1);
        img2 = findViewById(R.id.img2);

        util = new SystemFunUtil(this);

        mSelect.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                util.openCamera(SystemFunUtil.SYSTEM_SELECT_IMAGE,1);
            }
        });

        mRun.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                int w = bitmap.getWidth(), h = bitmap.getHeight();
                int[] pix = new int[w * h];
                bitmap.getPixels(pix, 0, w, 0, 0, w, h);
                int [] resultPixes = OpencvUtil.gray(pix,w,h);
                Bitmap result = Bitmap.createBitmap(w,h, Bitmap.Config.RGB_565);
                result.setPixels(resultPixes, 0, w, 0, 0,w, h);

                img2.setImageBitmap(result);
            }
        });

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


                bitmap = BitmapFactory.decodeFile(path);
                img1.setImageBitmap(bitmap);

            }
        }

    }

}
