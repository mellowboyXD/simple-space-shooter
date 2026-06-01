#ifndef DEBUG_H
#define DEBUG_H

typedef enum {L_INFO, L_WARN, L_ERROR} LogLevel;

#define LOG(logLevel, fmt, ...) __log(logLevel, __FILE__, __LINE__, \
                fmt __VA_OPT__(,) __VA_ARGS__)

#ifdef DEBUG

void __log(LogLevel logLevel, const char *file, int line, const char *fmt, ...);

#else
        #define __log(...) ((void) 0)
#endif // DEBUG

#endif // DEBUG_H
