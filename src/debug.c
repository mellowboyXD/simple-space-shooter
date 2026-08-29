#ifdef DEBUG

#include "debug.h"

#include <stdio.h>
#include <stdarg.h>

void debug_log(LogLevel logLevel, const char *file, int line, const char *fmt,
	       ...)
{
	switch (logLevel) {
	case L_INFO:
		fprintf(stdout, "[INFO]: ");
		break;
	case L_WARN:
		fprintf(stdout, "[WARN]: ");
		break;
	case L_ERROR:
		fprintf(stderr, "[ERROR]: ");
		break;
        case L_DEBUG:
                fprintf(stdout, "[DEBUG]: ");
                break;
	}

	FILE *stream = logLevel == L_ERROR ? stderr : stdout;
	fprintf(stream, "%s:%d - ", file, line);

	va_list args;
	va_start(args, fmt);
	vfprintf(stream, fmt, args);
	va_end(args);

	fprintf(stream, "\n");
}

#endif /* DEBUG */
