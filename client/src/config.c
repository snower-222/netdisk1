#include "../include/func.h"

void read_config(const char *filename, Config *config) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Error opening config file");
        exit(EXIT_FAILURE);
    }

    char line[128];
    while (fgets(line, sizeof(line), file)) {
        if (strncmp(line, "ip =", 4) == 0) {
            sscanf(line, "ip = %s", config->ip);
        } else if (strncmp(line, "port =", 6) == 0) {
            sscanf(line, "port = %d", &config->port);
        } else if (strncmp(line, "work_dir =", 10) == 0) {
            sscanf(line, "work_dir = %s", config->work_dir);
        } else if (strncmp(line, "log_level =", 11) == 0) {
            char level[16];
            sscanf(line, "log_level = %s", level);
            if (strcmp(level, "error") == 0) {
                config->log_level = LOG_LEVEL_ERROR;
            } else if (strcmp(level, "warning") == 0) {
                config->log_level = LOG_LEVEL_WARNING;
            } else if (strcmp(level, "info") == 0) {
                config->log_level = LOG_LEVEL_INFO;
            } else {
                fprintf(stderr, "Unknown log level: %s\n", level);
                exit(EXIT_FAILURE);
            }
        }
    }

    fclose(file);
}

