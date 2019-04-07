#include <jni.h>
#include <string>
#include <android/log.h>

extern "C" JNIEXPORT jstring JNICALL
Java_com_example_foxmap_1native_11_MainActivity_stringFromJNI(
        JNIEnv *env,
        jobject ) {
    std::string hello = "Hello";
    return env->NewStringUTF(hello.c_str());
    __android_log_print(ANDROID_LOG_DEBUG, "ABC", "\n");
}