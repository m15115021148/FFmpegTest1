package com.geek.ffmpegtest1;

import android.app.Activity;
import android.app.ProgressDialog;
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
import android.widget.Toast;

import com.geek.ffmpegtest1.util.FileUtil;
import com.geek.ffmpegtest1.util.SystemFunUtil;

import java.io.File;
import java.io.FileInputStream;
import java.util.ArrayList;
import java.util.List;

public class OpencvActivity extends AppCompatActivity {
    private String TAG = OpencvActivity.class.getSimpleName();
    private TextView mSelect,mRun;
    private ImageView img1,img2,img3;
    private SystemFunUtil util;
    private Bitmap bitmap ,bitmap1;
    private List<String> mList = new ArrayList<>();
    private String savePath;
    private ProgressDialog progressDialog;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_opencv);
        mSelect = findViewById(R.id.select);
        mRun = findViewById(R.id.run);
        img1 = findViewById(R.id.img1);
        img2 = findViewById(R.id.img2);
        img3 = findViewById(R.id.img3);

        util = new SystemFunUtil(this);

        mSelect.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                util.openCamera(SystemFunUtil.SYSTEM_SELECT_IMAGE,1);
            }
        });

        img1.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                util.openCamera(SystemFunUtil.SYSTEM_SELECT_IMAGE,2);
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

        progressDialog = new ProgressDialog(this);
        progressDialog.setCancelable(false);

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
                mList.add(0,path);

            }
            if (requestCode == 2){
                Uri uriVideo = data.getData();
                String path = util.getImageAbsolutePath(uriVideo);
                Log.d(TAG,"path-->"+path);
                if (TextUtils.isEmpty(path)){
                    return;
                }


                bitmap1 = BitmapFactory.decodeFile(path);
                img3.setImageBitmap(bitmap1);
                mList.add(1,path);
            }
        }

    }

    public void onSplite(View view) {
        savePath = FileUtil.getFolderName(mList.get(0));
        savePath = savePath + "IMG_MERGE.jpg";
        if (mList.size()>0){
            OpencvUtil.execute(mList.get(0), mList.get(1), savePath, new OpencvUtil.OpenCVRunListener() {
                @Override
                public void onStart() {
                    progressDialog.show();
                }

                @Override
                public void onEnd(double result) {
                    progressDialog.dismiss();

                    Toast.makeText(OpencvActivity.this,"时间："+result,Toast.LENGTH_SHORT).show();

                    Bitmap bitmap = BitmapFactory.decodeFile(savePath);
                    img2.setImageBitmap(bitmap);
                }
            });
        }
    }


    /**
     * 获取指定文件大小
     * @param
     * @return
     * @throws Exception
     */
    private long getFileSize(File file) throws Exception
    {
        long size = 0;
        if (file.exists()){
            FileInputStream fis = null;
            fis = new FileInputStream(file);
            size = fis.available();
        }
        else{
            file.createNewFile();
            Log.e("获取文件大小","文件不存在!");
        }
        return size;
    }

}
