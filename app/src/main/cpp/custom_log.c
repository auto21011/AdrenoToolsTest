#include "custom_log.h"

static FILE* log_file = NULL;
static pthread_mutex_t log_mutex = PTHREAD_MUTEX_INITIALIZER;

void log_init(const char* path) {
    pthread_mutex_lock(&log_mutex);
    if (log_file) {
        fclose(log_file);
    }
    log_file = fopen(path, "a");  // 追加模式
    if (log_file) {
        // 设置行缓冲，减少磁盘 I/O 频率
        setvbuf(log_file, NULL, _IOLBF, 0);
    }
    pthread_mutex_unlock(&log_mutex);
}

void log_close() {
    pthread_mutex_lock(&log_mutex);
    if (log_file) {
        fclose(log_file);
        log_file = NULL;
    }
    pthread_mutex_unlock(&log_mutex);
}

int log_to_file(int prio, const char* tag, const char* fmt, ...) {
    // 如果需要同时保留 logcat 输出，可以取消下面这行注释
    // __android_log_print(prio, tag, fmt, ...);

    // 获取当前时间
    time_t now = time(NULL);
    struct tm* tm_info = localtime(&now);
    char time_buf[20];
    strftime(time_buf, sizeof(time_buf), "%Y-%m-%d %H:%M:%S", tm_info);

    // 将优先级转换为字符串
    const char* prio_str;
    switch (prio) {
        case ANDROID_LOG_VERBOSE: prio_str = "VERBOSE"; break;
        case ANDROID_LOG_DEBUG:   prio_str = "DEBUG";   break;
        case ANDROID_LOG_INFO:    prio_str = "INFO";    break;
        case ANDROID_LOG_WARN:    prio_str = "WARN";    break;
        case ANDROID_LOG_ERROR:   prio_str = "ERROR";   break;
        case ANDROID_LOG_FATAL:   prio_str = "FATAL";   break;
        default:                   prio_str = "UNKNOWN"; break;
    }

    pthread_mutex_lock(&log_mutex);
    if (log_file) {
        // 先打印时间、优先级、TAG
        fprintf(log_file, "%s [%s] [%s] ", time_buf, prio_str, tag);

        // 打印用户消息
        va_list args;
        va_start(args, fmt);
        vfprintf(log_file, fmt, args);
        va_end(args);

        // 换行
        fprintf(log_file, "\n");
    }
    pthread_mutex_unlock(&log_mutex);
    return 0;
}
