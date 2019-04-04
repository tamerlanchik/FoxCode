#include <jni.h>
#include <string>

extern "C" JNIEXPORT jstring JNICALL
Java_com_example_foxmap_1native_11_MainActivity_stringFromJNI(
        JNIEnv *env,
        jobject ) {
    std::string hello = "Hello";
    return env->NewStringUTF(hello.c_str());
}