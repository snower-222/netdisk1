#include "../include/func.h"

int is_exist_file(const client_path_t *path, const char *name){

    char dest_path[128];
    memset(dest_path, 0, sizeof(dest_path));

    strcpy(dest_path, path->client_root_path);
    
    printf("tools.c : is_exist_file : %s\n", dest_path);

    DIR *p_dir = opendir(dest_path);
    int type = -1;  // -1 文件不存在

    struct dirent *p_dirent;
    while((p_dirent = readdir(p_dir)) != NULL){
        if(strcmp(p_dirent->d_name , name) == 0){
            type = p_dirent->d_type;
            printf("name : %s, type : %d\n", p_dirent->d_name, p_dirent->d_type);
        }
    }

    switch(type){
    case 0:
        return DT_UNKNOWN;  // 未知文件
    case 1:
        return DT_FIFO;  // 有名管道文件
    case 2:
        return DT_CHR;  // 字符设备文件
    case 4:
        return DT_DIR;  // 目录文件
    case 6:
        return DT_BLK;  // 块设备文件
    case 8:
        return DT_REG;  // 普通文件
    case 10:
        return DT_LNK;  // 符号链接文件
    case 12:
        return DT_SOCK;  // 套接字文件
    }

    return -1;
}
