#include "debug.h"

#include <stdarg.h>
#include <stdio.h>

#ifdef DEBUG
void __log(LogLevel logLevel, const char *file, int line, const char *fmt, ...)
{
        switch(logLevel) {
                case L_INFO:
                        fprintf(stdout, "[INFO]: ");
                        break;
                case L_WARN:
                        fprintf(stdout, "[WARN]: ");
                        break;
                case L_ERROR:
                        fprintf(stderr, "[ERROR]: ");
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

#endif // DEBUG_H
