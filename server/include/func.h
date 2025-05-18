#ifndef _FUNC_H_
#define _FUNC_H_
#include "func_struct.h"

// 读取配置文件信息
void read_config(const char *filename, Config *config);

// 初始化日志系统
int init_logger(Logger *logger, const char *log_path, LogLevel level);

// 记录日志消息
void log_message(Logger *logger, LogLevel level, const char *file, const char *function, int line, const char *format, ...);

// 记录错误信息
void log_error(Logger *logger, const char *file, const char *function, int line, const char *format, ...);

// 关闭日志系统
void close_logger(Logger *logger);

// 封装日志记录函数
// 用户登录
void log_user_login(Logger *logger, const char *username);
// 用户退出
void log_user_logout(Logger *logger, const char *username);
// 用户传输文件暂停
void log_file_transfer_stop(Logger *logger, const char *username, const char *filename);
// 传输文件出错
void log_file_transfer_error(Logger *logger, const char *username, const char *filename);
// 删除文件
void log_file_delete(Logger *logger, const char *username, const char *filename);
// 程序出错
void log_proccess_error(Logger *logger, const char *func_name);

#define LOG_ERROR(logger, format, ...) log_error(logger, __FILE__, __FUNCTION__, __LINE__, format, ##__VA_ARGS__)
#define LOG_WARNING(logger, format, ...) log_message(logger, LOG_LEVEL_WARNING, __FILE__, __FUNCTION__, __LINE__, format, ##__VA_ARGS__)
#define LOG_INFO(logger, format, ...) log_message(logger, LOG_LEVEL_INFO, __FILE__, __FUNCTION__, __LINE__, format, ##__VA_ARGS__)


// 初始化线程池
int init_pool(pool_t *pool, int num);

// 初始化socket
int init_socket(int *socket_fd, char *ip, char *port);
int initTCP(int *ptr_socketfd, Config *config);
// epoll监听
int add_epoll(int epoll_fd, int fd);

// 子线程处理逻辑
void *thread_main(void *p);

// 任务队列
int enqueue(queue_t *p_queue, int net_fd);
int dequeue(queue_t *p_queue);

// 判断文件是否存在，类型是什么
int is_exist_file(const server_path_t *path, const char *name);

// 丢弃倒数第二个字符串之后的
void cut_path(char *path);

// 计算/
int count_slashes(const char *str);


// 切换目录
int command_cd(int net_fd, command_t *command, server_path_t *path);

// 创建文件夹
int command_mkdir(int net_fd, command_t *command, const server_path_t *path);

// pwd
int command_pwd(int net_fd, const server_path_t *path);

// ls 
int command_ls(int net_fd, const server_path_t *path);

int command_rm(int net_fd, const server_path_t *path, const command_t *command);

int command_login(int net_fd, server_path_t *path, const command_t *command);

int command_puts(int net_fd, const server_path_t *path, const command_t *command);
int command_gets(int net_fd, const server_path_t *path, const command_t *command);
#endif
