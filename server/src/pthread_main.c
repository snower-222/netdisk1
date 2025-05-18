#include "../include/func.h"

void *thread_main(void *p){

    pool_t *pool = (pool_t *)p;

    while(1){

        pthread_mutex_lock(&pool->lock);
        while(pool->queue.size <= 0 && pool->exit_flag == 0){
            pthread_cond_wait(&pool->cond, &pool->lock);
        }

        if(pool->exit_flag == 1){
            pthread_mutex_unlock(&pool->lock);
            pthread_exit(NULL);
        }

        int net_fd = pool->queue.head->net_fd;

        dequeue(&pool->queue);
        pthread_mutex_unlock(&pool->lock);

        server_path_t path;
        memset(&path, 0, sizeof(path));
        strcpy(path.server_root_path, "/home/zhanjian/C/net_disk/server/disk_root/");

        command_t command;
        while(1){
            memset(&command, 0, sizeof(0));
            int res = recv(net_fd, &command, sizeof(command_t), MSG_WAITALL);
            if(res == 0){
                printf("断开连接\n");
                break;
            }
            if(strcmp(command.command, "login") == 0){
                printf("login逻辑\n");
                command_login(net_fd, &path, &command);
                continue;
            }
            if(strcmp(command.command, "mkdir") == 0){
                printf("mkdir逻辑\n");
                command_mkdir(net_fd, &command, &path);
                continue;
            }
            if(strcmp(command.command, "ls") == 0) {
                printf("ls逻辑\n");
                command_ls(net_fd, &path);
                continue;
            }

            if(strcmp(command.command, "pwd") == 0) {
                printf("pwd逻辑\n");
                command_pwd(net_fd, &path);
                continue;
            }

            if(strcmp(command.command, "cd") == 0) {
                printf("cd逻辑\n");
                command_cd(net_fd, &command, &path);
                continue;
            }

            if(strcmp(command.command, "puts") == 0) {
                printf("puts逻辑\n");
                command_puts(net_fd, &path, &command);
                continue;
            }

            if(strcmp(command.command, "gets") == 0) {
                printf("gets逻辑\n");
                command_gets(net_fd, &path, &command);
                continue;
            }

            if(strcmp(command.command, "rm") == 0) {
                printf("rm逻辑\n");
                command_rm(net_fd, &path, &command);
                continue;
            }

        }

        close(net_fd);
    }
}
