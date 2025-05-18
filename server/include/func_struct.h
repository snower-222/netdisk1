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
    char mysql_user[64];
    char mysql_password[64];
    char mysql_db[64];
    LogLevel log_level;
} Config;

// 路径
typedef struct server_path_s{
    char server_root_path[64];  // 服务端工作目录
    char user_path[64];  // 用户虚拟目录
}server_path_t;

// 定义队列节点
typedef struct node_s{
    int net_fd;
    struct node_s *p_next;
}node_t;

// 定义队列
typedef struct queue_s{
    node_t *head;
    node_t *end;
    int size;
}queue_t;

typedef struct pool_s{
    pthread_t *list;  // 保存线程id
    int num;  // 线程池中线程个数
    queue_t queue;  // 任务队列 存储net_fd
    pthread_mutex_t lock;  // 锁
    pthread_cond_t cond;  // 条件变量
    int exit_flag;  // 退出标记位：0正常 1退出
}pool_t;

typedef struct command_s{
    int size;  // 需要读取的数目
    int command_size;  // 指令长度
    char command[8];  // 指令
    int parameter_size;  // 参数长度
    char parameter[64];  // 参数
}command_t;

#endif
