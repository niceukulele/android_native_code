LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE := fpalgorithm
LOCAL_SRC_FILES := $(LOCAL_PATH)/lib/libFpCore.a
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := simple
LOCAL_SRC_FILES := $(LOCAL_PATH)/src/simple.cpp
include $(BUILD_STATIC_LIBRARY)

include $(CLEAR_VARS)

SRC_FILE_PATH := $(LOCAL_PATH)/src

ifeq ($(APP_ABI),arm64-v8a)
LOCAL_CFLAGS +=  -DFASYNC_SUPPORTED -ffast-math -g -DHAVE_PTHREADS
else
LOCAL_CFLAGS +=  -DFASYNC_SUPPORTED  -ffast-math -g
#LOCAL_CFLAGS += -DHAVE_PTHREADS
LOCAL_CFLAGS +=  -DHAVE_ANDROID_OS
endif

LOCAL_MODULE := algorithm

LOCAL_CXXFLAGS += -DHAVE_PTHREADS

LOCAL_C_INCLUDES += $(LOCAL_PATH)/inc $(LOCAL_PATH)/src 
# for logging
LOCAL_LDLIBS    += -llog
# for native asset manager
LOCAL_LDLIBS    += -landroid

LOCAL_SRC_FILES := $(SRC_FILE_PATH)/main.cpp \
                  $(SRC_FILE_PATH)/simple.cpp
                  # $(SRC_FILE_PATH)/main.cpp 
                  # $(SRC_FILE_PATH)/cpl_assert.c   \
		  # $(SRC_FILE_PATH)/cpl_memory.c   \
                  # $(SRC_FILE_PATH)/cpl_stdio.c    \
                  # $(SRC_FILE_PATH)/cpl_string.c   \
                  # $(SRC_FILE_PATH)/cpl_thread.c   \
                  # $(SRC_FILE_PATH)/gx_algorithm.c \
                  # $(SRC_FILE_PATH)/gx_memory.c

#LOCAL_LDFLAGS := $(LOCAL_PATH)/lib/libFpCore.a
#LOCAL_STATIC_LIBRARIES := FpCore

#LOCAL_LDFLAGS += -L$(LOCAL_PATH)/lib
#LOCAL_LDLIBS += -lutils \
		#-lcutils \
		#-lbinder
MY_LIB_DIR = $(LOCAL_PATH)/lib
LOCAL_LDFLAGS += $(MY_LIB_DIR)/libbinder.so
#LOCAL_LDFLAGS += $(MY_LIB_DIR)/libcutils.so
LOCAL_LDFLAGS += $(MY_LIB_DIR)/libutils.so
#LOCAL_SHARED_LIBRARIES := libbinder \
			  #libutils \
			 # libcutils

include $(BUILD_EXECUTABLE)


