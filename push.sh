adb root
adb remount
adb push libs/armeabi/algorithm /system/
adb shell chmod 777 /system/algorithm
