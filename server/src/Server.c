#include "../include/func.h"

int pipe_fd[2];

void func(int num) {
    write(pipe_fd[1], "1", 1);
}

int main(int argc, char *argv[]) {
    // 管道通信
    if (pipe(pipe_fd) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    if (fork() != 0) {
        // 主进程 异步拉起同步

        // 注册信号
        if (signal(SIGINT, func) == SIG_ERR) {
            perror("signal");
            exit(EXIT_FAILURE);
        }

        wait(NULL);
        exit(0);
    }

    setpgid(0, 0);

    Config config;
    memset(&config, 0, sizeof(config));
    read_config(argv[1], &config);

    //Logger my_logger;
    //init_logger(&my_logger, "./log/server.log", config.log_level);

    pool_t pool;
    init_pool(&pool, 3);

    int socket_fd;
    initTCP(&socket_fd, &config);

    int epoll_fd = epoll_create(1);
    if (epoll_fd == -1) {
        perror("epoll_create");
        exit(EXIT_FAILURE);
    }

    struct epoll_event event;
    event.events = EPOLLIN;

    event.data.fd = socket_fd;
    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, socket_fd, &event) == -1) {
        perror("epoll_ctl: socket_fd");
        exit(EXIT_FAILURE);
    }

    event.data.fd = pipe_fd[0];
    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, pipe_fd[0], &event) == -1) {
        perror("epoll_ctl: pipe_fd[0]");
        exit(EXIT_FAILURE);
    }

    while (1) {
        struct epoll_event events[5];
        int epoll_num = epoll_wait(epoll_fd, events, 5, -1);

        //log_error(&my_logger, "main.c", "epoll_wait", 70, "Server", "监听返回", "");

        for (int i = 0; i < epoll_num; i++) {
            if (events[i].data.fd == pipe_fd[0]) {
                // 注册信号-->父进程
                // 有序退出
                char buf[60] = {0};
                read(pipe_fd[0], buf, sizeof(buf));

                pthread_mutex_lock(&pool.lock);
                pool.exit_flag = 1;
                pthread_cond_broadcast(&pool.cond);
                pthread_mutex_unlock(&pool.lock);

                for (int j = 0; j < pool.num; j++) {
                    pthread_join(pool.list[j], NULL);
                }

                //close_logger(&my_logger);
                printf("Bye\n");
                pthread_exit(NULL);
            } else if (events[i].data.fd == socket_fd) {
                // 客户端连接


                int net_fd = accept(socket_fd, NULL, NULL);
                if (net_fd == -1) {
                    perror("accept");
                    continue;
                }
                printf("Client connected: %d\n", net_fd);

                pthread_mutex_lock(&pool.lock);
                enqueue(&pool.queue, net_fd);
                pthread_cond_signal(&pool.cond);
                pthread_mutex_unlock(&pool.lock);
            }
        }
    }
    return 0;
}

