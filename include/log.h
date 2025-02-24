#ifndef LOG_H
#define LOG_H

#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <time.h>

extern FILE *log_file;
extern struct tm *time_info;
extern time_t current_time;

typedef enum {
    LOG_INFO,
    LOG_WARN,
    LOG_DEBUG,
    LOG_ERROR
} log_level;

void log_init();
void log_uninit();
void logprint(log_level level, const char *fmt, ...);

#endif // LOG_H
