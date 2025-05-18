#include "../include/func.h"

int is_exist_file(const server_path_t *path, const char *name){

    char dest_path[128];
    memset(dest_path, 0, sizeof(dest_path));

    strcpy(dest_path, path->server_root_path);
    strcat(dest_path, path->user_path);

    DIR *p_dir = opendir(dest_path);
    int type = -1;  // 不存在

    struct dirent *p_dirent;
    while((p_dirent = readdir(p_dir)) != NULL){
        if(strcmp(p_dirent->d_name, name) == 0){
            type = p_dirent->d_type;
            printf("name: %s type: %d\n", p_dirent->d_name, type);
        }
    }
    closedir(p_dir);
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


void cut_path(char *path) {
    int len = strlen(path);
    int slash_count = 0;

    // 从字符串末尾向前遍历
    for (int i = len - 1; i >= 0; i--) {
        if (path[i] == '/') {
            slash_count++;
            if (slash_count == 2) {
                // 找到倒数第二个 /
                // 从 i+1 到 len-1 的位置全部置为 0
                for (int j = i + 1; j < len; j++) {
                    path[j] = '\0';
                }
                break;
            }
        }
    }
}

int count_slashes(const char *str) {
    int count = 0;
    while (*str != '\0') {
        if (*str == '/') {
            count++;
        }
        str++;
    }
    return count;
}
