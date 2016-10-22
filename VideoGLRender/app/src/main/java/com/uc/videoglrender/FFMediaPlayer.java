package com.uc.videoglrender;

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

    public int init(String name, int type){
        return nativeInit(name, type);
    }

    public void start(){
        nativeStart();
    }

    public void stop() {
        nativeStop();
    }





    public native String  stringFromJNI();

    public native void setStringToJni(String value);

    public native int nativeInit(String name, int type);

    public native void nativeStart();

    public native void nativeStop();

    public static native void initRender(int width, int height);
    public static native void step();


    static {
        System.loadLibrary("ffmpeg");
        System.loadLibrary("glrender");
    }
}
