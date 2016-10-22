package com.example.videodecode;

import android.os.Bundle;
import android.support.design.widget.FloatingActionButton;
import android.support.design.widget.Snackbar;
import android.support.v7.app.AppCompatActivity;
import android.support.v7.widget.Toolbar;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.RadioButton;
import android.widget.RadioGroup;
import android.widget.Spinner;
import android.widget.TextView;

import java.util.logging.Logger;

import static com.example.videodecode.R.id.fab;

public class PlayActivity extends AppCompatActivity {


    private FFMediaPlayer ffMedia = null;

    private  Spinner videosp;
    private  RadioGroup rg;
    private RadioButton decodeR;
    private RadioButton readR;


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_play);
        Toolbar toolbar = (Toolbar) findViewById(R.id.toolbar);
        setSupportActionBar(toolbar);

        ffMedia = new FFMediaPlayer(this);

        TextView tv = (TextView)findViewById(R.id.testText);
        tv.setText(ffMedia.getString());

        videosp = (Spinner)findViewById(R.id.videosp);
        //rg = (RadioGroup)findViewById(R.id.thread);
        decodeR = (RadioButton)findViewById(R.id.decode1);
        readR = (RadioButton)findViewById(R.id.decode2);
        decodeR.setChecked(true);


//        ffMedia.init("/sdcard/");

        Button h264 = (Button) findViewById(R.id.h264Btn);
        h264.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {

                String path = videosp.getSelectedItem().toString();

                int type = 0;
                if (decodeR.isChecked()) {
                    type = 1;
                }else if (readR.isChecked()) {
                    type = 2;
                }

                Log.e("test", "path = " + path + "type is " + type);
                ffMedia.init(path, type);

                //ffMedia.start();
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
