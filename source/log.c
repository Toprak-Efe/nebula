#include "log.h"

FILE *g_log_file = NULL;
struct tm *g_time_info = NULL;
time_t g_current_time = 0;

void log_init() {
    g_log_file = fopen("log.txt", "a+");
    if (g_log_file == NULL) {
        fprintf(stderr, "Failed to open log file\n");
        exit(1);
    }
    atexit(log_uninit);
}

void log_uninit() {
    fclose(g_log_file);
}

void logprint(log_level level, const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    time(&g_current_time);
    g_time_info = localtime(&g_current_time);
    fprintf(g_log_file, "[%d-%d-%d %d:%d:%02d] ", 
        g_time_info->tm_year + 1900,
        g_time_info->tm_mon + 1,
        g_time_info->tm_mday,
        g_time_info->tm_hour,
        g_time_info->tm_min,
        g_time_info->tm_sec
    );
    switch (level) {
        case LOG_INFO:
            fprintf(g_log_file, "INFO: ");
            break;
        case LOG_WARN:
            fprintf(g_log_file, "WARN: ");
            break;
        case LOG_DEBUG:
            fprintf(g_log_file, "DEBUG: ");
            break;
        case LOG_ERROR:
            fprintf(g_log_file, "ERROR: ");
            break;
    }
    vfprintf(g_log_file, fmt, args);
    va_end(args);
    fprintf(g_log_file, "\n");

    #ifdef DEBUG
    va_start(args, fmt);
    fprintf(stdout, "[%d-%d-%d %d:%d:%02d] ", 
        g_time_info->tm_year + 1900,
        g_time_info->tm_mon + 1,
        g_time_info->tm_mday,
        g_time_info->tm_hour,
        g_time_info->tm_min,
        g_time_info->tm_sec
    );
    switch (level) {
        case LOG_INFO:
            fprintf(stdout, SET_COLOR_GREEN);
            fprintf(stdout, "INFO: ");
            fprintf(stdout, RESET_COLOR);
            break;
        case LOG_WARN:
            fprintf(stdout, SET_COLOR_YELLOW);
            fprintf(stdout, "WARN: ");
            fprintf(stdout, RESET_COLOR);
            break;
        case LOG_DEBUG:
            fprintf(stdout, SET_COLOR_GREEN);
            fprintf(stdout, "DEBUG: ");
            fprintf(stdout, RESET_COLOR);
            break;
        case LOG_ERROR:
            fprintf(stdout, SET_COLOR_RED);
            fprintf(stdout, "ERROR: ");
            fprintf(stdout, RESET_COLOR);
            break;
    }
    vfprintf(stdout, fmt, args);
    va_end(args);
    fprintf(stdout, "\n");
    #endif
}
