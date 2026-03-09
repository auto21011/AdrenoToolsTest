// custom_log.h
#ifndef CUSTOM_LOG_H
#define CUSTOM_LOG_H

#include <android/log.h>
#include <stdio.h>
#include <pthread.h>
#include <stdarg.h>
#include <time.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

// 初始化日志文件，需传入文件路径（例如从 JNI 获取的内部存储路径）
void log_init(const char* path);

// 关闭日志文件
void log_close();

// 自定义日志写入函数，用法与 __android_log_print 相同
int log_to_file(int prio, const char* tag, const char* fmt, ...);

#ifdef __cplusplus
}
#endif
#endif
