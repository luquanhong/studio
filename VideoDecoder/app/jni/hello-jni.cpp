/*
 * Copyright (C) 2009 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */
#include <string.h>
#include <jni.h>
#include <android/log.h>

#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/sendfile.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <iostream>
 #include <string>
#include <vector>

#include <errno.h>

#include "looper.h"

extern "C" {
#include <libavutil/avutil.h>
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
}

using namespace std;

#define  LOG_TAG    "VideoDecode"
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)


AVFormatContext *pFormatCtx = NULL;
int             audioStream, videoStream, delay_time, videoFlag = 0;
AVCodecContext  *aCodecCtx;
AVCodec         *aCodec;
AVFrame         *aFrame;
AVPacket        packet;
int  frameFinished = 0;


////////////////////////////

JavaVM *myVm;
const static int64_t kUsPerSecond = 1000000LL; // 10e6

enum {
    kMsgStart,
    kMsgPause,
    kMsgResume,
    kMsgPauseAck,
    kMsgSeek,
};

class mylooper: public looper {
    virtual void handle(int what, void* obj);
};

static mylooper *mlooper = NULL;

void doCodecWork(void *d) {

    char* x = (char*)malloc(1024000);
    LOGE("doCodecWork malloc 1024");
    mlooper->post(kMsgStart, 0);
}

void mylooper::handle(int what, void* obj) {
    switch (what) {
        case kMsgStart:
            doCodecWork(0);
            break;
    }
}

/////////////////////////////////////////////////////////
int64_t getRealTimeUs()
{
    struct timeval tv;
    gettimeofday(&tv, 0);

    return (int64_t)tv.tv_sec * kUsPerSecond + (int64_t)tv.tv_usec;
}

 vector<string> split(string str,string pattern)
 {
     string::size_type pos;
     vector<string> result;
     str += pattern;
     int size=str.size();

     for(int i=0; i<size; i++)
     {
         pos=str.find(pattern,i);
         if(pos<size)
         {
             string s=str.substr(i,pos-i);
             result.push_back(s);
             i=pos+pattern.size()-1;
         }
     }
     return result;
 }


 void *read_thread_fun(void* arg)
 {

    LOGE( "read_thread_fun enter");

    int ret;
    int count = 0;

    aFrame = avcodec_alloc_frame();
    if(aFrame == NULL) ;

    while(videoFlag != -1)
    {
        if(av_read_frame(pFormatCtx, &packet) < 0)
            break;

        if(packet.stream_index == videoStream)
        {
            ret = avcodec_decode_video2(aCodecCtx, aFrame, &frameFinished, &packet);
            if(ret > 0 && frameFinished)
            {
                int data_size = av_samples_get_buffer_size(
                        aFrame->linesize,aCodecCtx->channels,
                        aFrame->nb_samples,aCodecCtx->sample_fmt, 0);
                LOGI("videoDecodec  :%d",data_size);
            }

        }
#if 0
        usleep(50000);
        while(videoFlag != 0) {
            if(videoFlag == 1)//暂停 {
                sleep(1);
            }else if(videoFlag == -1) //停止 {
                break;
            }
        }
#endif
        av_free_packet(&packet);
    }
    av_free(aFrame);
 }


int jni_nativeInit( JNIEnv* env,jobject thiz, jstring fileName )
{
    //string v((char*)env->GetStringUTFChars(value, 0));
    //LOGE( "jni_nativeInit  %s", v.c_str());

    const char* local_title = env->GetStringUTFChars(fileName, NULL);
    LOGE( "jni_nativeInit  local_title %s", local_title);

    av_register_all();//注册所有支持的文件格式以及编解码器
    /*
     *只读取文件头，并不会填充流信息
     */
    if(avformat_open_input(&pFormatCtx, local_title, NULL, NULL) != 0)
        return -1;

    /*
     *获取文件中的流信息，此函数会读取packet，并确定文件中所有流信息，
     *设置pFormatCtx->streams指向文件中的流，但此函数并不会改变文件指针，
     *读取的packet会给后面的解码进行处理。
     */
    if(avformat_find_stream_info(pFormatCtx, NULL) < 0)
        return -1;

    /*
     *输出文件的信息，也就是我们在使用ffmpeg时能够看到的文件详细信息，
     *第二个参数指定输出哪条流的信息，-1代表ffmpeg自己选择。最后一个参数用于
     *指定dump的是不是输出文件，我们的dump是输入文件，因此一定要为0
     */
    av_dump_format(pFormatCtx, -1, local_title, 0);

    int i = 0;
    for(i=0; i< pFormatCtx->nb_streams; i++)
    {
        if(pFormatCtx->streams[i]->codec->codec_type == AVMEDIA_TYPE_AUDIO){
            audioStream = i;
            break;
        } else if(pFormatCtx->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO){
           videoStream = i;
           break;
       }
    }

    if(videoStream < 0)
        return -1;
    aCodecCtx = pFormatCtx->streams[videoStream]->codec;
    aCodec = avcodec_find_decoder(aCodecCtx->codec_id);
    if(avcodec_open2(aCodecCtx, aCodec, NULL) < 0)
        return -1;

    env->ReleaseStringUTFChars(fileName, local_title);

    return 0;
}

void jni_nativeStart( JNIEnv* env,jobject thiz )
{
    LOGE("%s", __FUNCTION__);

    pthread_t id1;
    LOGE( "pthread_create read");
    pthread_create(&id1, NULL, &read_thread_fun, NULL);

    //pthread_t id2;
    //LOGE( "pthread_create decode");
    //pthread_create(&id2, NULL, &decode_thread_fun, NULL);
}

void jni_nativeStop( JNIEnv* env,jobject thiz )
{
    videoFlag = -1;
    LOGE("%s", __FUNCTION__);
    avcodec_close(aCodecCtx);
    avformat_close_input(&pFormatCtx);
}

jstring jni_stringFromJNI( JNIEnv* env, jobject thiz )
{

#if 1
    pthread_t id;
    //LOGE( "pthread_create ");
    //pthread_create(&id, NULL, &thread_fun, NULL);
#else
    //mlooper = new mylooper();
    //mlooper->post(kMsgStart, 0);
#endif
    return env->NewStringUTF("Hello from JNI !  Compiled with ABI hello world " );
}


void jni_setStringToJni( JNIEnv* env,jobject thiz, jstring value )
{
	string v((char*)env->GetStringUTFChars(value, 0));
	LOGE( "jni_setStringToJni  %d", v.length());
	if(v.c_str() ){
		LOGE( "jni_setStringToJni  11");
	} else {
		LOGE( "jni_setStringToJni  00");
	}
}

static const char *classPathName = "com/example/videodecode/FFMediaPlayer";

static JNINativeMethod methods[] = {
				{"stringFromJNI",		"()Ljava/lang/String;",	    (void*)jni_stringFromJNI},
				{"setStringToJni",		"(Ljava/lang/String;)V",	(void*)jni_setStringToJni},
				{"nativeInit",  		"(Ljava/lang/String;)I",	(void*)jni_nativeInit},
				{"nativeStart",	    	"()V",	                    (void*)jni_nativeStart},
				{"nativeStop",		    "()V",	                    (void*)jni_nativeStop},
};


static int registerNativeMethods(JNIEnv* env, const char* className,
                                JNINativeMethod* gMethods, int numMethods)
{
	jclass clazz;

	clazz = env->FindClass( className);	//!< get java class instance from my java class
	if (clazz == NULL) {
			return JNI_FALSE;
	}

	if ( env->RegisterNatives( clazz, gMethods, numMethods) < 0) {
			return JNI_FALSE;
	}
	return JNI_TRUE;
}

static int registerNatives(JNIEnv* env)
{
    if (!registerNativeMethods(env, classPathName,methods, sizeof(methods) / sizeof(methods[0]))) {
        return JNI_FALSE;
    }

    return JNI_TRUE;
}

jint JNI_OnLoad(JavaVM* vm, void* reserved)
{
    JNIEnv* env = NULL;
    jint result = -1;

    if (vm->GetEnv((void**)&env, JNI_VERSION_1_4) != JNI_OK) {
        return result;
    }
    registerNatives(env);
    return JNI_VERSION_1_4;
}


void JNI_OnUnload(JavaVM* vm, void* reserved) {

    if (mlooper) {
        mlooper->quit();
        delete mlooper;
        mlooper = NULL;
    }
}




