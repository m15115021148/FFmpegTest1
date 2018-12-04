LOCAL_PATH:=$(call my-dir)

#opencv start
include $(CLEAR_VARS)
OpenCV_INSTALL_MODULES := on
OpenCV_CAMERA_MODULES := off
OPENCV_LIB_TYPE :=STATIC

ifeq ("$(wildcard $(OPENCV_MK_PATH))","")
include $(LOCAL_PATH)/native/jni/OpenCV.mk
else
include $(OPENCV_MK_PATH)
endif
#opecv end

include $(CLEAR_VARS)
LOCAL_MODULE := ffmpeg
LOCAL_SRC_FILES := ffmpeg/lib/$(TARGET_ARCH_ABI)/libffmpeg.so
LOCAL_C_INCLUDES :=/Users/cmm/Desktop/work/ffmpeg
include $(PREBUILT_SHARED_LIBRARY)

#include $(CLEAR_VARS)
LOCAL_MODULE := core
LOCAL_SRC_FILES :=  core_opencv.cpp \
#                    core_ffmpeg.c \
                    ffmpeg/ffmpeg.c \
                    ffmpeg/ffmpeg_opt.c \
                    ffmpeg/cmdutils.c \
                    ffmpeg/ffmpeg_filter.c \
                    ffmpeg/ffmpeg_hw.c


LOCAL_LDLIBS +=  -lm -llog -lz -ldl

LOCAL_C_INCLUDES := $(LOCAL_PATH)/util \
                    $(LOCAL_PATH)/native/jni/include \
#                    Users/cmm/Desktop/work/ffmpeg


LOCAL_SHARED_LIBRARIES := ffmpeg

include $(BUILD_SHARED_LIBRARY)
