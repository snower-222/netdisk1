#include "../include/func.h"


int init_pool(pool_t *pool, int num){

    pool->list = (pthread_t *)calloc(num, sizeof(pthread_t));
    pool->num = num;

    memset(&pool->queue, 0, sizeof(pool->queue));

    pthread_mutex_init(&pool->lock, NULL);
    pthread_cond_init(&pool->cond, NULL);

    pool->exit_flag = 0;

    for(int i = 0; i < num; i++){
        pthread_create(&pool->list[i], NULL, thread_main, pool);
    }
    
    return 0;
}

int initTCP(int *ptr_socketfd, Config *config){
    *ptr_socketfd = socket(AF_INET, SOCK_STREAM, 0);
    ERROR_CHECK(*ptr_socketfd, -1, "initTCP");

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(config->port);
    addr.sin_addr.s_addr = inet_addr(config->ip);

    // 设置端口快速重启
    int reuse = 1;
    int ret_setopt = setsockopt(*ptr_socketfd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));
    ERROR_CHECK(ret_setopt, -1, "setsockopt");

    int ret_bind = bind(*ptr_socketfd, (struct sockaddr *)&addr, sizeof(addr));
    ERROR_CHECK(ret_bind, -1, "bind");
    listen(*ptr_socketfd, 16);
    return 0;
}

int init_socket(int *socket_fd, char *ip, char *port){

    *socket_fd = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in sockaddr;
    sockaddr.sin_family = AF_INET;
    sockaddr.sin_port = htons(atoi(port));
    sockaddr.sin_addr.s_addr = inet_addr(ip);

    bind(*socket_fd, (struct sockaddr*)&sockaddr, sizeof(sockaddr));

    listen(*socket_fd, 10);

    return 0;
}

int add_epoll(int epoll_fd, int fd){

    struct epoll_event event;
    event.events = EPOLLIN;
    event.data.fd = fd;

    epoll_ctl(epoll_fd, EPOLL_CTL_ADD, fd, &event);

    return 0;
}
