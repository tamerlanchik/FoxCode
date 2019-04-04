//
// Created by Andrey on 04.04.2019.
//
#include <jni.h>
#include <string>



/*extern "C"
JNIEXPORT jint JNICALL
getInt(JNIEnv *env, jobject instance);

static JNINativeMethod gMethodRegistry[] = {
        { "getInt", "()I", (void *)&getInt }
};
JNIEXPORT jint JNI_OnLoad(JavaVM* pVM, void* reserved) {
// Инициализировать хранилище.

    JNIEnv *env;    //environment
    if ( pVM->GetEnv((void**) &env, JNI_VERSION_1_6) != JNI_OK )
    { abort(); }
    jclass JavaTest = env->FindClass("com/example/foxmap_native_1/Test");
    if (JavaTest == NULL) { abort(); }

    if(sizeof(gMethodRegistry) > 0)
        env->RegisterNatives(JavaTest, gMethodRegistry, sizeof(gMethodRegistry)/sizeof(gMethodRegistry[0]));

    return JNI_VERSION_1_6;
}

JNIEXPORT jint JNICALL
getInt(JNIEnv *env, jobject instance) {

    // TODO
    return 0;

}*/