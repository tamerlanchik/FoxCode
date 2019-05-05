//
// Created by Andrey on 06.04.2019.
//

#ifndef FOXMAPNATIVE_1_LOGNATIVE_H
#define FOXMAPNATIVE_1_LOGNATIVE_H
#ifdef __ANDROID__
	#include <android/log.h>
	#include <jni.h>
#else
	#include <stdio.h>
	#include <stdarg.h>
	enum { ANDROID_LOG_ERROR, ANDROID_LOG_DEBUG, ANDROID_LOG_INFO };
#endif


class Log {
public:
    static void error(const char* tag, const char* message, ...);
    static void debug(const char* tag, const char* message, ...);
    static void info(const char* tag, const char* message, ...);
private:
    static void print(const int prioritet, const char* tag, const char* message, ...);
};


#endif //FOXMAPNATIVE_1_LOGNATIVE_H