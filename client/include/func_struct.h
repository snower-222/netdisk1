#ifndef _FUNC_STRUCT_H_
#define _FUNC_STRUCT_H_
#include "head.h"

// 定义日志级别
typedef enum LogLevel{
    LOG_LEVEL_ERROR,
    LOG_LEVEL_WARNING,
    LOG_LEVEL_INFO
} LogLevel;

// 日志系统结构
typedef struct Logger{
    FILE *log_file;
    LogLevel level;
} Logger;

// 配置文件信息结构体 
typedef struct Config{
    char ip[16];
    int port;
    LogLevel log_level;
    char work_dir[64];
} Config;

typedef struct client_path_s{
    char client_root_path[64];
    char user_path[64];
}client_path_t;

typedef struct command_s{
    int size;  // 需要读取的数目
    int command_size;  // 指令长度
    char command[8];  // 指令
    int parameter1_size;  // 参数长度
    char parameter1[64];  // 参数
}command_t;
#endif
