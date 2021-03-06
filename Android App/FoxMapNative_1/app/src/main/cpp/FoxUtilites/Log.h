//
// Created by Andrey on 06.04.2019.
//

#ifndef FOXMAPNATIVE_1_LOGNATIVE_H
#define FOXMAPNATIVE_1_LOGNATIVE_H
#include <string>
#ifdef __ANDROID__
	#include <android/log.h>
	#ifdef NDEBUG
	#define DEBUG
	#endif
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
	static void error(const char* tag, const std::string& message, ...);
	static void debug(const char* tag, const std::string& message, ...);
	static void info(const char* tag, const std::string& message, ...);
private:
    static void print(const int prioritet, const char* tag, const char* message, ...);
};


#endif //FOXMAPNATIVE_1_LOGNATIVE_H