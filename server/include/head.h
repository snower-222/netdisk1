
#ifndef _HEAD_H_
#define _HEAD_H_
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
#include <ctype.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <pwd.h>
#include <grp.h>
#include <dirent.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <signal.h>
#include <pthread.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/epoll.h>
#include <sys/sendfile.h>
#include <mysql/mysql.h>
#include <errno.h>
#include <stdarg.h>

#define ERROR_CHECK(ret, error_flag, msg) \
    do { \
        if ((ret) == (error_flag)) { \
            perror(msg);  \
            /* exit(-1); */  \
        } \
    } while (0)

#define ARGS_CHECK(argc, expected) \
    do { \
        if ((argc) != (expected)) { \
            fprintf(stderr, "args num error!\n"); \
            /* exit(-1); */  \
        } \
    } while (0)

#define THREAD_ERROR_CHECK(ret, msg) \
    do { \
        if ((ret) != 0) {  \
            fprintf(stderr, "%s: %s\n", msg, strerror(ret)); \
            /* exit(-1); */  \
        }   \
    } while (0)
#endif
