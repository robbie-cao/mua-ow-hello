#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "log.h"

static int _log_level = 0;

void log_out(LOG_LEVEL level, const char * fmt, ...)
{
	va_list ap;

	if (level == LOG_LEVEL_OFF || level < _log_level) {
		return;
	}

	va_start(ap, fmt);
	vfprintf(stdout, fmt, ap);
	va_end(ap);
}

void log_set_level(LOG_LEVEL level)
{
	_log_level = level;
}


void log_init(void)
{
}

/* vim: set ts=4 sw=4 tw=0 noexpandtab nolist : */
