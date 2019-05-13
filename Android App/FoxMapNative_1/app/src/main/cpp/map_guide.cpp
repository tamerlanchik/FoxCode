//
// Created by Andrey on 04.04.2019.
//
#include <jni.h>
#include <cstdlib>



JNIEXPORT jint JNICALL
getRoute(JNIEnv *env, jobject instance);

//Таблица обьявления нативных функций. Позволяет использовать не стрёмные длинные названия, а нормальные.
//Обьявление каждой функции имеет вид:
//      { name, signature, funcPointer }
//Read more: https://www.programering.com/a/MTO0UTMwATI.html
static JNINativeMethod gMethodRegistry[] = {
        { "getInt", "()I", (void *)&getRoute }
};

//Функция, вызываемая в самом начале и только один раз.
JNIEXPORT jint JNI_OnLoad(JavaVM* pVM, void* reserved) {
    // Инициализировать хранилище.

    JNIEnv *env;    //environment
    //Инициализируем Java Native Environment
    if ( pVM->GetEnv((void**) &env, JNI_VERSION_1_6) != JNI_OK )
    { abort(); }

    //Подключаем Java-классы
    jclass JavaTest = env->FindClass("com/example/foxmap_native_1/MapGuide");
    if (JavaTest == NULL) { abort(); }

    //Добавляем методы, обьявленные в Java-классах.
    if(sizeof(gMethodRegistry) > 0)
        env->RegisterNatives(JavaTest, gMethodRegistry, sizeof(gMethodRegistry)/sizeof(gMethodRegistry[0]));

    return JNI_VERSION_1_6;
}

//Функция поиска маршрута.
//ACHTUNG!! Сие есмь заготовкаЪ
JNIEXPORT jint JNICALL
getRoute(JNIEnv *env, jobject instance) {
    return 0;
}
