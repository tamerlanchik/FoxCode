#include <jni.h>
#include <cstdlib>
#include "FoxUtilites/Log.h"
const char TAG[] = "storage";


extern "C" {
JNIEXPORT void JNICALL
Java_com_example_foxmap_1native_11_StorageMasterJNI_init(
        JNIEnv *env, jobject instance) {
    Log::debug(TAG, "Init storage master");
}

JNIEXPORT void JNICALL
Java_com_example_foxmap_1native_11_StorageMasterJNI_inflateDatabase(
        JNIEnv *env, jobject instance) {
}
}