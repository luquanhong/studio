package com.example.videodecode;

import android.content.Context;

/**
 * Created by jack on 16-10-20.
 */

public class FFMediaPlayer {

    private Context mContext;

    public FFMediaPlayer(Context context) {
        this.mContext = context;
    }

    public String getString(){
       return stringFromJNI();
   }

    public int init(String name){
        return nativeInit(name);
    }

    public void start(){
        nativeStart();
    }

    public void stop() {
        nativeStop();
    }

    public native String  stringFromJNI();

    public native void setStringToJni(String value);

    public native int nativeInit(String name);

    public native void nativeStart();

    public native void nativeStop();


    static {
        System.loadLibrary("ffmpeg");
        System.loadLibrary("hello-jni");
    }
}
