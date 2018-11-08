package com.geek.ffmpegtest1;

import android.os.Environment;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Toast;

public class MainActivity extends AppCompatActivity {
    private String base = Environment.getExternalStorageDirectory().getPath();
    private String filePath = base + "/input.txt";
    private String outPath = base + "/input3.mp4";

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
    }

    public void onRun(View view) {

        FFmpegUtil.execute(CommandsUtil.mergeVideo(filePath, outPath), new FFmpegUtil.FFmpegRunListener() {
            @Override
            public void onStart() {

            }

            @Override
            public void onEnd(int result) {
                if(result == 0){
                    Toast.makeText(MainActivity.this, "命令行执行完成", Toast.LENGTH_SHORT).show();
                }
            }
        });

    }
}
