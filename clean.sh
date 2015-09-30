#!/bin/bash
NDK_DIR=/home/wangbo/workspace/tools/android-ndk-r10d
${NDK_DIR}/ndk-build clean NDK_PROJECT_PATH=./ APP_BUILD_SCRIPT=./Android.mk NDK_APPLICATION_MK=./Application.mk -B NDK_OUT=./libs
#cp libs/x86_64/algorithm a.out

#cp libs/x86/algorithm a.out
echo "build finish"
