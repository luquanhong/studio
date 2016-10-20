# Copyright (C) 2009 The Android Open Source Project
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
LOCAL_PATH := $(call my-dir)



###############
# FFmpeg settings
###############

# libffmpeg prebuilt
include $(CLEAR_VARS)

LOCAL_MODULE := ffmpeg-prebuilt
LOCAL_SRC_FILES := $(LOCAL_PATH)/ffmpeg/libffmpeg.so
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/ffmpeg/include
#LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/ffmpeg/include

include $(PREBUILT_SHARED_LIBRARY)




include $(CLEAR_VARS)

TARGET_ARCH_ABI=armeabi-v7a
LOCAL_ARM_MODE=arm
LOCAL_ARM_NEON=true
LOCAL_ALLOW_UNDEFINED_SYMBOLS=false

LOCAL_MODULE    := hello-jni

#LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/ffmpeg/include
LOCAL_C_INCLUDES := $(LOCAL_PATH)/ffmpeg/include

$(info LOCAL_EXPORT_C_INCLUDES = $(LOCAL_C_INCLUDES))

LOCAL_SRC_FILES :=  looper.cpp \
                    hello-jni.cpp

LOCAL_LDLIBS    := -llog  -L$(LOCAL_PATH)/ffmpeg -lffmpeg

$(info LOCAL_LDLIBS = $(LOCAL_LDLIBS))

#LOCAL_ADDRESS_SANITIZE := true


#LOCAL_LDFLAGS   :=  -fsanitize=address
#LOCAL_ARM_MODE := arm
#LOCAL_CFLAGS    +=   -fno-omit-frame-pointer



#LOCAL_ARM_MODE := arm
#LOCAL_CFLAGS += -fsanitize=address 

#LOCAL_LDLIBS += -lclang_rt.asan-arm-android
#LOCAL_LDFLAGS += -L/home/jack/Android/asan/

 
#LOCAL_CLANG:=true
#LOCAL_SANITIZE:=address
#LOCAL_MODULE_RELATIVE_PATH := asan

include $(BUILD_SHARED_LIBRARY)
