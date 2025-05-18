#include "../include/func.h"

int init_logger(Logger *logger, const char *log_path, LogLevel level) {
    logger->log_file = fopen(log_path, "a");
    if (logger->log_file == NULL) {
        perror("Error opening log file");
        return -1;
    }
    logger->level = level;
    return 0;
}

void log_message(Logger *logger, LogLevel level, const char *file, const char *function, int line, const char *format, ...) {
    if (level > logger->level) {
        return;
    }

    time_t now;
    time(&now);
    char time_str[20];
    strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S", localtime(&now));

    const char *level_str;
    switch (level) {
        case LOG_LEVEL_ERROR:
            level_str = "ERROR";
            break;
        case LOG_LEVEL_WARNING:
            level_str = "WARNING";
            break;
        case LOG_LEVEL_INFO:
            level_str = "INFO";
            break;
        default:
            level_str = "UNKNOWN";
    }

    fprintf(logger->log_file, "[%s] [%s] [%s:%s:%d] ", time_str, level_str, file, function, line);

    va_list args;
    va_start(args, format);
    vfprintf(logger->log_file, format, args);
    va_end(args);
    fprintf(logger->log_file, "\n");
    fflush(logger->log_file);
}

void log_error(Logger *logger, const char *file, const char *function, int line, const char *format, ...) {
    if (LOG_LEVEL_ERROR > logger->level) {
        return;
    }

    time_t now;
    time(&now);
    char time_str[20];
    strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S", localtime(&now));

    fprintf(logger->log_file, "[%s] [ERROR] [%s:%s:%d] ", time_str, file, function, line);

    va_list args;
    va_start(args, format);
    vfprintf(logger->log_file, format, args);
    va_end(args);

    fprintf(logger->log_file, "\n");
    fflush(logger->log_file);
}

void close_logger(Logger *logger) {
    if (logger->log_file != NULL) {
        fclose(logger->log_file);
        logger->log_file = NULL;
    }
}

// 封装日志记录函数
void log_user_login(Logger *logger, const char *username) {
    MY_LOG_INFO(logger, "用户: %s 登录", username);
}

void log_user_logout(Logger *logger, const char *username) {
    MY_LOG_INFO(logger, "用户: %s 退出", username);
}

void log_file_transfer_stop(Logger *logger, const char *username, const char *filename) {
    MY_LOG_WARNING(logger, "用户: %s 在传输文件: %s时中断", username, filename);
}

void log_file_transfer_error(Logger *logger, const char *username, const char *filename) {
    MY_LOG_ERROR(logger, "用户: %s 传输文件: %s失败", username, filename);
}

void log_file_delete(Logger *logger, const char *username, const char *filename) {
    MY_LOG_WARNING(logger, "用户: %s 删除文件: %s", username, filename);
}

void log_proccess_error(Logger *logger, const char *func_name){
    MY_LOG_ERROR(logger, "程序运行出错：%s", func_name);
}
