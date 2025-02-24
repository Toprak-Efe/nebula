#include "log.h"

FILE *log_file = NULL;
struct tm *time_info = NULL;
time_t current_time = 0;

void log_init() {
    log_file = fopen("log.txt", "a+");
    if (log_file == NULL) {
        fprintf(stderr, "Failed to open log file\n");
        exit(1);
    }
    atexit(log_uninit);
}

void log_uninit() {
    fclose(log_file);
}

void logprint(log_level level, const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    time(&current_time);
    time_info = localtime(&current_time);
    fprintf(log_file, "[%d-%d-%d %d:%d:%d] ", 
        time_info->tm_year + 1900,
        time_info->tm_mon + 1,
        time_info->tm_mday,
        time_info->tm_hour,
        time_info->tm_min,
        time_info->tm_sec
    );
    switch (level) {
        case LOG_INFO:
            fprintf(log_file, "INFO: ");
            break;
        case LOG_WARN:
            fprintf(log_file, "WARN: ");
            break;
        case LOG_DEBUG:
            fprintf(log_file, "DEBUG: ");
            break;
        case LOG_ERROR:
            fprintf(log_file, "ERROR: ");
            break;
    }
    vfprintf(log_file, fmt, args);
    va_end(args);
    fprintf(log_file, "\n");

    #ifdef DEBUG
    va_start(args, fmt);
    fprintf(stdout, "[%d-%d-%d %d:%d:%d] ", 
        time_info->tm_year + 1900,
        time_info->tm_mon + 1,
        time_info->tm_mday,
        time_info->tm_hour,
        time_info->tm_min,
        time_info->tm_sec
    );
    switch (level) {
        case LOG_INFO:
            fprintf(stdout, "INFO: ");
            break;
        case LOG_WARN:
            fprintf(stdout, "WARN: ");
            break;
        case LOG_DEBUG:
            fprintf(stdout, "DEBUG: ");
            break;
        case LOG_ERROR:
            fprintf(stdout, "ERROR: ");
            break;
    }
    vfprintf(stdout, fmt, args);
    va_end(args);
    fprintf(stdout, "\n");
    #endif
}
