//
// Created by Andrey on 06.04.2019.
#define DEBUG
#include "Log.h"

void Log::error(const char *tag, const char *message, ...) {
    Log::print(ANDROID_LOG_ERROR, tag, message);
}

void Log::debug(const char *tag, const char *message ...) {
    Log::print(ANDROID_LOG_DEBUG, tag, message);
}

void Log::info(const char *tag, const char *message, ...) {
    Log::print(ANDROID_LOG_INFO, tag, message);
}

void Log::error(const char *tag, const std::string& message, ...) {
	Log::print(ANDROID_LOG_ERROR, tag, message.c_str());
}

void Log::debug(const char *tag, const std::string& message ...) {
	Log::print(ANDROID_LOG_DEBUG, tag, message.c_str());
}

void Log::info(const char *tag, const std::string& message, ...) {
	Log::print(ANDROID_LOG_INFO, tag, message.c_str());
}

#ifdef __ANDROID__
void Log::print(const int prio, const char *tag, const char *message, ...) {
	va_list var_args;
	va_start(var_args, message);    //собирает все аргументы
	__android_log_vprint(prio, tag, message, var_args);
	__android_log_print(prio, tag, "\n");
	va_end(var_args);               //парный к va_start(). должен быть вызван перед завершением функции
}
#else
void Log::print(const int prio, const char *tag, const char *message, ...) {
	printf("%s: %s\n", tag, message);
}
#endif


