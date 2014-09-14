#include <stdio.h>
#include "../include/log.h"
#include <stdarg.h>

static void log_do(int level, const char *fmt, va_list ap);

void log_info(const char *fmt, ...)
{
	va_list		ap;
	va_start(ap, fmt);
	log_do(LOG_INFO, fmt, ap);
	va_end(ap);
	return;
}

void log_debug(const char *fmt, ...)
{
	va_list		ap;
	va_start(ap, fmt);
	log_do(LOG_DEBUG, fmt, ap);
	va_end(ap);
	return;
}

void log_warn(const char *fmt, ...)
{
	va_list		ap;
	va_start(ap, fmt);
	log_do(LOG_WARN, fmt, ap);
	va_end(ap);
	return;
}

void log_err(const char *fmt, ...)
{
	va_list		ap;
	va_start(ap, fmt);
	log_do(LOG_ERR, fmt, ap);
	va_end(ap);
	return;

}

static void log_do(int level, const char *fmt, va_list ap)
{
	vprintf(fmt, ap);
}
