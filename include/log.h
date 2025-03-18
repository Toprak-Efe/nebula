#ifndef LOG_H
#define LOG_H

#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <time.h>

extern FILE *g_log_file;
extern struct tm *g_time_info;
extern time_t g_current_time;

#define RESET_COLOR      "\033[0m"
#define SET_COLOR_RED    "\033[0;31m"
#define SET_COLOR_YELLOW "\033[0;33m"
#define SET_COLOR_GREEN  "\033[0;32m"

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
