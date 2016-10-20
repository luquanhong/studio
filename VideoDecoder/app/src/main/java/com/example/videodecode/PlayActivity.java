package com.example.videodecode;

import android.os.Bundle;
import android.support.design.widget.FloatingActionButton;
import android.support.design.widget.Snackbar;
import android.support.v7.app.AppCompatActivity;
import android.support.v7.widget.Toolbar;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;

import static com.example.videodecode.R.id.fab;

public class PlayActivity extends AppCompatActivity {


    private FFMediaPlayer ffMedia = null;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_play);
        Toolbar toolbar = (Toolbar) findViewById(R.id.toolbar);
        setSupportActionBar(toolbar);

        ffMedia = new FFMediaPlayer(this);

        TextView tv = (TextView)findViewById(R.id.testText);
        tv.setText(ffMedia.getString());



//        ffMedia.init("/sdcard/");

        Button h264 = (Button) findViewById(R.id.h264Btn);
        h264.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {

                ffMedia.init("/sdcard/zuihou_480p_264.mp4");

                ffMedia.start();
            }
        });

        Button h265 = (Button)findViewById(R.id.h265Btn);
        h265.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                ffMedia.init("/sdcard/zuihou_480p_265.mp4");

                ffMedia.start();
            }
        });

        Button stop = (Button)findViewById(R.id.StopBtn);
        stop.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {

                ffMedia.stop();
            }
        });


        FloatingActionButton fab = (FloatingActionButton) findViewById(R.id.fab);
        fab.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                Snackbar.make(view, "Replace with your own action", Snackbar.LENGTH_LONG)
                        .setAction("Action", null).show();

                ffMedia.start();
            }
        });
    }



}
