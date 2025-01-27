#include <pthread.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <unistd.h>

#include "logger.h"

#include "../main/config.h"

#define LOG_LENGTH_MAX 1024

#define BLACK  "\033[0;30m"
#define RED    "\033[0;31m"
#define GREEN  "\033[0;32m"
#define YELLOW "\033[0;33m"
#define BLUE   "\033[0;34m"
#define PURPLE "\033[0;35m"
#define CYAN   "\033[0;36m"
#define WHITE  "\033[0;37m"
#define RESET  "\033[0m"

__attribute__((used))
static void
colorize_start(char* output_buffer, const char* color)
{
    strncat(output_buffer, color, 8);
}

__attribute__((used))
static void
colorize_stop(char* output_buffer)
{
    strncat(output_buffer, RESET, 5);
}

void
ginger_log(uint8_t log_level, const char* fmt, ...)
{
    if (!global_config_get_verbosity() && log_level < INFO) {
        return;
    }

    char log_buffer[LOG_LENGTH_MAX] = {0};
    strncat(log_buffer, "[", 2);

    if (log_level == INFO) {
        colorize_start(log_buffer, GREEN);
        strncat(log_buffer, "INFO", 5);
    }
    else if (log_level == DEBUG) {
        colorize_start(log_buffer, BLUE);
        strncat(log_buffer, "DEBUG", 6);
    }
    else if (log_level == WARNING) {
        colorize_start(log_buffer, YELLOW);
        strncat(log_buffer, "WARNING", 8);
    }
    else if (log_level == ERROR) {
        colorize_start(log_buffer, RED);
        strncat(log_buffer, "ERROR", 6);
    }

    colorize_stop(log_buffer);
    strncat(log_buffer, "] ", 3);

    // Log thread ID.
    const int thread_info_str_sz = 21; //"[Thread 0x100000000] "
    char pid_str[thread_info_str_sz];
    memset(pid_str, 0, sizeof(thread_info_str_sz));
    const pid_t x = syscall(__NR_gettid);
    sprintf(pid_str, "[Thread 0x%x] ", x);
    strncat(log_buffer, pid_str, thread_info_str_sz);

    const uint64_t free_space = LOG_LENGTH_MAX - (strlen(log_buffer) + 1);

    char str_buffer[free_space];
    memset(str_buffer, 0, free_space);

    va_list ap;
    va_start(ap, fmt);
    vsnprintf(str_buffer, free_space, fmt, ap);
    va_end(ap);

    // This might be slow.
    strncat(log_buffer, str_buffer, free_space);

    printf("%s", log_buffer);
}
