#include "../include/func.h"

int command_cd(int net_fd, command_t *command, server_path_t *path){

    int res = is_exist_file(path, command->parameter);
    char error_info[256] = {0};
    send(net_fd, &res, sizeof(int), MSG_NOSIGNAL);
    if(res != DT_DIR){
        strcpy(error_info, command->command);
        strcat(error_info, ": ");
        strcat(error_info, command->parameter);
        strcat(error_info, ": ");
        int error_len = strlen(error_info);
        send(net_fd, &error_len, sizeof(int), MSG_NOSIGNAL);
        send(net_fd, error_info, error_len, MSG_NOSIGNAL);
    }else{
        if(strcmp(command->parameter, ".") == 0){
            int path_len = strlen(path->user_path);
            send(net_fd, &path_len, sizeof(int), MSG_NOSIGNAL);
            send(net_fd, path->user_path, path_len, MSG_NOSIGNAL);
        }else if(strcmp(command->parameter, "..") == 0){
            if(count_slashes(path->user_path) < 2){
                memset(path->user_path, 0, sizeof(0));
            }else{
                cut_path(path->user_path); 
            }
            int path_len = strlen(path->user_path);
            send(net_fd, &path_len, sizeof(int), MSG_NOSIGNAL);
            if(path_len != 0){
                send(net_fd, path->user_path, path_len, MSG_NOSIGNAL);
            }
        }else{
            strcat(path->user_path, command->parameter);
            strcat(path->user_path, "/");
            int res_len = strlen(path->user_path);
            send(net_fd, &res_len, sizeof(int), MSG_NOSIGNAL);
            send(net_fd, path->user_path, res_len, MSG_NOSIGNAL);
        }
    }

    return 0;
}

int command_mkdir(int net_fd, command_t *command, const server_path_t *path){

    char dest_path[256] = {0};
    strcpy(dest_path, path->server_root_path);
    strcat(dest_path, path->user_path);
    strcat(dest_path, command->parameter);
    mkdir(dest_path, 0777);
    return 0;
}

int command_pwd(int net_fd, const server_path_t *path){
    char dest_path[128] = {0};
    strcpy(dest_path, path->user_path);
    int len = strlen(dest_path);
    send(net_fd, &len, sizeof(int), MSG_NOSIGNAL);
    send(net_fd, dest_path, len, MSG_NOSIGNAL);
    return 0;
}


int command_ls(int net_fd, const server_path_t *path){
    char dest_path[256] = {0};
    strcpy(dest_path, path->server_root_path);
    strcat(dest_path, path->user_path);
    DIR *p_dir = opendir(dest_path);
    int flag = -1;

    struct dirent *p_dirent;
    while((p_dirent = readdir(p_dir)) != NULL){
        if((strcmp(p_dirent->d_name, ".")) == 0 ||
           (strcmp(p_dirent->d_name, "..")) == 0){
            continue;
        }
        int len = strlen(p_dirent->d_name);
        send(net_fd, &len, sizeof(int), MSG_NOSIGNAL);
        send(net_fd, p_dirent->d_name, len, MSG_NOSIGNAL);
    }

    send(net_fd, &flag, sizeof(int), MSG_NOSIGNAL);
    closedir(p_dir);
    return 0;
}

int command_rm(int net_fd, const server_path_t *path, const command_t *command){
    int res = is_exist_file(path, command->parameter);
    char dest_path[256] = {0};
    strcpy(dest_path, path->server_root_path);
    strcat(dest_path, path->user_path);
    strcat(dest_path, command->parameter);
    if(res != 4){
        remove(dest_path);
    }else{
        printf("该功能未完善\n");
    }
    return 0;
}

int command_login(int net_fd, server_path_t *path, const command_t *command){
    int res = is_exist_file(path, command->parameter);
    send(net_fd, &res, sizeof(int), MSG_NOSIGNAL);
    if(res != 4){
        return 0;
    }
    strcat(path->server_root_path, command->parameter);
    strcat(path->server_root_path, "/");
    int len = strlen(command->parameter);
    send(net_fd, &len, sizeof(int), MSG_NOSIGNAL);
    send(net_fd, command->parameter, len, MSG_NOSIGNAL);
    return 0;
}

int command_puts(int net_fd, const server_path_t *path, const command_t *command){
    int client_res_exist = 0;
    char dest_path[256];
    memset(dest_path, 0, sizeof(dest_path));
    strcpy(dest_path, path->server_root_path);
    strcat(dest_path, path->user_path);
    strcat(dest_path, "/");
    strcat(dest_path, command->parameter);
    recv(net_fd, &client_res_exist, sizeof(int), MSG_NOSIGNAL);
    if(client_res_exist == -1){
        return 0;
    }
    if(client_res_exist == DT_REG){
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
    if(client_res_exist == DT_DIR){
        // 目录文件
        printf("该功能未完善\n");
    }
    return 0;
}


int command_gets(int net_fd, const server_path_t *path, const command_t *command){
    char dest_path[256];
    memset(dest_path, 0, sizeof(dest_path));
    strcpy(dest_path, path->server_root_path);
    strcat(dest_path, path->user_path);
    strcat(dest_path, command->parameter);
    printf("dest_path : %s\n", dest_path);
    int res_exist = is_exist_file(path, command->parameter);
    send(net_fd, &res_exist, sizeof(int), MSG_NOSIGNAL);
    if(res_exist == -1){
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

