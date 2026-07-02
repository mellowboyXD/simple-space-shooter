#ifndef DEBUG_H
#define DEBUG_H

typedef enum {L_INFO, L_WARN, L_ERROR} LogLevel;

#ifdef DEBUG

void ___log(LogLevel logLevel, const char *file, int line, const char *fmt, ...);

#define LOG(logLevel, fmt, ...) ___log(logLevel, __FILE__, __LINE__, \
                fmt __VA_OPT__(,) __VA_ARGS__)

#else

#define LOG(logLevel, ...) ((void) 0)

#endif // DEBUG

#endif // DEBUG_H
