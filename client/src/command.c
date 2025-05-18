#include "../include/func.h"

int command_cd(int net_fd, client_path_t *path){
    int NUM = 0;
    recv(net_fd, &NUM, sizeof(int), MSG_WAITALL);
    if(NUM != 4){
        int length = 0;
        char str[] = "No such file or directory";
        char error_info[128] = {0};
        recv(net_fd, &length, sizeof(int), MSG_WAITALL);
        recv(net_fd, &error_info, length, MSG_WAITALL);
        strcat(error_info, str);
        printf("%s\n", error_info);
    }else{
        int len = 0;
        char new_path[128] ={0};
        recv(net_fd, &len, sizeof(int), MSG_WAITALL);
        if(len != 0){
            recv(net_fd, new_path, len, MSG_WAITALL);
            memset(path->user_path, 0, sizeof(path->user_path));
            strcpy(path->user_path, new_path);
        }else{
            memset(path->user_path, len, sizeof(path->user_path));
        }
        printf("%s\n", path->user_path);
    }

    return 0;
}

int command_pwd(int net_fd){
    char dest_path[128] = {0};
    int len = 0;
    recv(net_fd, &len,sizeof(int), MSG_WAITALL);
    recv(net_fd, dest_path, len, MSG_WAITALL);
    printf("%s\n", dest_path);
    return 0;
}

int command_ls(int net_fd){
    int len = 0;
    char file_name[64] = {0};
    while(1){

        recv(net_fd, &len, sizeof(int), MSG_NOSIGNAL);
        if(len == -1){
            printf("\n");
            return 0;
        }
        recv(net_fd, file_name, len, MSG_WAITALL);
        printf("%s ", file_name);

        len = 0;
        memset(file_name, 0, sizeof(file_name));

    }
    return 0;
}

int command_login(int net_fd, char *user, int *flag){
    int res = 0;
    recv(net_fd, &res, sizeof(int), MSG_WAITALL);
    if(res != 4){
        printf("No such user\n");
        return 0;
    }
    int len = 0;
    recv(net_fd, &len, sizeof(int), MSG_WAITALL);
    recv(net_fd, user, len, MSG_WAITALL);
    printf("Login success\n");
    *flag = 1;
    return 0;
}

int command_puts(int net_fd, const client_path_t *path, command_t *command){
    char dest_path[256];
    memset(dest_path, 0, sizeof(dest_path));
    strcpy(dest_path, path->client_root_path);
    strcat(dest_path, command->parameter1);
    int res_exist = is_exist_file(path, command->parameter1);
    send(net_fd, &res_exist, sizeof(int), MSG_NOSIGNAL);
    if(res_exist == -1){
        printf("No such file\n");
        return 0;
    }
    if(res_exist == DT_REG){
        // 普通文件
        int file_fd = open(dest_path, O_RDWR);
        ERROR_CHECK(file_fd, -1, "open");

        struct stat st;
        memset(&st, 0, sizeof(st));
        fstat(file_fd, &st);

        off_t file_size = st.st_size;
        off_t server_file_size  = 0;
        recv(net_fd, &server_file_size, sizeof(off_t), MSG_WAITALL);
        int flag = 0;
        if(server_file_size == file_size){
            flag = 1;
        }
        if(server_file_size < file_size){
            flag = 2;
        }
        send(net_fd, &flag, sizeof(int), MSG_NOSIGNAL);
        if(flag == 1){
            printf("already existed\n");
            close(file_fd);
            return 0;
        }
        if(flag == 2){
            off_t residual_size = file_size - server_file_size;
            send(net_fd, &residual_size, sizeof(off_t), MSG_NOSIGNAL);
            lseek(net_fd, server_file_size, SEEK_SET);
            char buf[4096];
            int num = 0;
            while(num < residual_size){
                memset(buf, 0, sizeof(buf));
                int res = read(file_fd, buf, sizeof(buf));
                num += res;
                send(net_fd, &res, sizeof(int), MSG_NOSIGNAL);
                send(net_fd, buf, res, MSG_NOSIGNAL);
            }
            close(file_fd);
            return 0;
        }
    }
    if(res_exist == DT_DIR){
        // 目录文件
        printf("该功能未完善\n");
    }
    return 0;
}

int command_gets(int net_fd, const client_path_t *path, command_t *command){
    int server_res_exist = 0;
    char dest_path[256];
    memset(dest_path, 0, sizeof(dest_path));
    strcpy(dest_path, path->client_root_path);
    strcat(dest_path, command->parameter1);
    printf("dest_path : %s\n", dest_path);
    recv(net_fd, &server_res_exist, sizeof(int), MSG_NOSIGNAL);
    if(server_res_exist == -1){
        printf("No such file\n");
        return 0;
    }
    if(server_res_exist == DT_REG){
        // 普通文件
        int file_fd = open(dest_path, O_RDWR | O_CREAT, 0666);
        ERROR_CHECK(file_fd, -1, "open");

        struct stat st;
        memset(&st, 0, sizeof(st));
        fstat(file_fd, &st);

        off_t file_size = st.st_size;
        int flag = 0;
        send(net_fd, &file_size, sizeof(off_t), MSG_NOSIGNAL);
        recv(net_fd, &flag, sizeof(int), MSG_WAITALL);
        if(flag == 1){
            close(file_fd);
            return 0;
        }
        if(flag == 2){
            off_t residual_size = 0;
            recv(net_fd, &residual_size, sizeof(off_t), MSG_WAITALL);
            lseek(net_fd, file_size, SEEK_SET);  // 移动到最后写入
            char buf[4096];
            off_t num = 0; // 计录写入了多少
            while(num < residual_size){
                memset(buf, 0, sizeof(buf));
                int res = 0;
                recv(net_fd, &res, sizeof(int), MSG_WAITALL);
                num += res;
                recv(net_fd, buf, res, MSG_WAITALL);
                write(file_fd, buf, res);
            }
            close(file_fd);
            return 0;
        }
    }
    if(server_res_exist == DT_DIR){
        // 目录文件
        printf("该功能未完善\n");
    }
    return 0;
}
