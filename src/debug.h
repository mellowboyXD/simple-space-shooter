#ifndef DEBUG_H
#define DEBUG_H

typedef enum { L_INFO, L_WARN, L_ERROR } LogLevel;

#ifdef DEBUG
#include "raylib.h"

void debug_log(LogLevel logLevel, const char *file, int line, const char *fmt,
	       ...);

#define LOG(logLevel, fmt, ...) \
	debug_log(logLevel, __FILE__, __LINE__, fmt __VA_OPT__(, ) __VA_ARGS__)

void debug_window(Rectangle panel);

#define DEBUG_WINDOW(panel) debug_window((panel))

#else

#define LOG(logLevel, ...) ((void)0)

#define DEBUG_WINDOW(panel) ((void) 0)

#endif // DEBUG


#endif // DEBUG_H
