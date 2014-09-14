#ifndef _LOG_H
#define _LOG_H 1
#define LOG_DEBUG	1
#define LOG_INFO	2
#define LOG_WARN	3
#define LOG_ERR		4

void log_info(const char *fmt, ...);
void log_debug(const char *fmt, ...);
void log_warn(const char *fmt, ...);
void log_err(const char *fmt, ...);

#endif
