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

int main(int argc, char *argv[])
{
    Config config;
    memset(&config, 0, sizeof(config));
    read_config(argv[1], &config);

    int net_fd = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(config.port);
    addr.sin_addr.s_addr = inet_addr(config.ip);
    connect(net_fd, (struct sockaddr *)&addr, sizeof(addr));

    char str[128];
    command_t command;
    client_path_t path;
    memset(&path, 0,sizeof(path));
    strcpy(path.client_root_path, config.work_dir);

    Logger my_logger;
    init_logger(&my_logger, "./log/client.log", config.log_level);
    int flag = 0; //未登录 1 登录
    char user[16] = {0};
    while(1){
        if(flag == 1){
            if(strlen(path.user_path) == 0){
                printf("%s@:~$",user);
            }else{
                printf("%s@:~/%s$", user, path.user_path);
            }
        }else{
            printf("please login : \n");
        }
        memset(&str, 0, sizeof(str));
        memset(&command, 0, sizeof(command));
        fgets(str, sizeof(str), stdin);
        int res = dispose_command(str, &command);
        printf("%d %d %s %d %s\n", command.size, command.command_size,command.command, command.parameter1_size,command.parameter1);

        if(flag == 0){
            if(res == LOGIN){
                send(net_fd, &command, sizeof(command_t), MSG_NOSIGNAL);
                command_login(net_fd, user, &flag);
                log_user_logout(&my_logger, user);
            }
            continue;
        }
        switch(res){
        case LOGIN:
            printf("already logged in\n");
            break;
        case EXIT:
            log_user_logout(&my_logger, user);
            close_logger(&my_logger);
            close(net_fd);
            return 0;
        case 1:
            printf("path illegal\n");
            continue;
        case -1:
            printf("command error\n");
            continue;
        case LS:
            send(net_fd, &command, sizeof(command_t), MSG_NOSIGNAL);
            command_ls(net_fd);
            break;
        case PWD:
            send(net_fd, &command, sizeof(command_t), MSG_NOSIGNAL);
            command_pwd(net_fd);
            break;
        case PUTS:
            send(net_fd, &command, sizeof(command_t), MSG_NOSIGNAL);
            command_puts(net_fd, &path, &command);
            break;
        case GETS:
            send(net_fd, &command, sizeof(command_t), MSG_NOSIGNAL);
            command_gets(net_fd, &path, &command);
            break;
        case CD:
            send(net_fd, &command, sizeof(command_t), MSG_NOSIGNAL);
            command_cd(net_fd, &path);
            break;
        case MKDIR:
            send(net_fd, &command, sizeof(command_t), MSG_NOSIGNAL);
            break;
        case RM:
            send(net_fd, &command, sizeof(command_t), MSG_NOSIGNAL);
            break;
        }
    }
    return 0;
}

