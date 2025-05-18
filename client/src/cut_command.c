#include "../include/func.h"

#define LS 4
#define PWD 5
#define PUTS 6
#define GETS 7
#define CD 8
#define RM 9
#define MKDIR 11
#define LOGIN 12
#define EXIT 13

// 删除前面所有空格，中间空格保留一个
void remove_spaces(char *str) {
    int len = strlen(str);
    int i = 0, j = 0;
    int spaceFound = 0;

    // 跳过前面的所有空格
    while (isspace(str[i])) {
        i++;
    }

    for (; i < len; i++) {
        if (isspace(str[i])) {
            if (!spaceFound) {
                str[j++] = ' ';
                spaceFound = 1;
            }
        } else {
            str[j++] = str[i];
            spaceFound = 0;
        }
    }

    // 如果末尾有空格，删除它
    if (j > 0 && str[j-1] == ' ') {
        j--;
    }

    str[j] = '\0';
}

int is_valid_path(const char *path) {
    const char *illegal_chars = "<>:\"*?|!\\\0";

    while (*path) {
        if (strchr(illegal_chars, *path)) {
            return -1;  // 发现错误字符
        }
        path++;
    }
    return 0;
}

int dispose_command(char *str, command_t *command) {
    remove_spaces(str);
    if (is_valid_path(str) == -1) {
        return 1;
    }

    char *saveptr;
    const char delim[] = " ";  // 分隔符

    char *p = strtok_r(str, delim, &saveptr);

    command->size++;
    command->command_size = strlen(p);
    strcpy(command->command, p);

    p = strtok_r(NULL, delim, &saveptr);
    if(p == NULL){
        if(strcmp(command->command, "ls") == 0){
            return LS;
        }
        if(strcmp(command->command, "pwd") == 0){
            return PWD;
        }
        if(strcmp(command->command, "exit") == 0){
            return EXIT;
        }
        return -1;
    }
    command->size++;
    command->parameter1_size = strlen(p);
    strcpy(command->parameter1, p);

    p = strtok_r(NULL, delim, &saveptr);
    if(p == NULL){
        if(strcmp(command->command, "rm") == 0){
            return RM;
        }
        if(strcmp(command->command, "cd") == 0){
            return CD;
        }
        if(strcmp(command->command, "puts") == 0){
            return PUTS;
        }
        if(strcmp(command->command, "gets") == 0){
            return GETS;
        }
        if(strcmp(command->command, "mkdir") == 0){
            return MKDIR;
        }
        if(strcmp(command->command, "login") == 0){
            return LOGIN;
        }
    }

    return -1;
}

