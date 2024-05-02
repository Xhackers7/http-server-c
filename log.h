#pragma once
#include "stdio.h"
#include <stdarg.h>

#define Log_debug(module, ...) Log(level_debug, (char *)module, __VA_ARGS__)
#define Log_info(module, ...) Log(level_info, (char *)module, __VA_ARGS__)
#define Log_warning(module, ...) Log(level_warning, (char *)module, __VA_ARGS__)
#define Log_error(module, ...) Log(level_error, (char *)module, __VA_ARGS__)
#define Log_critical(module, ...) Log(level_critical, (char *)module, __VA_ARGS__)

typedef enum
{
    level_debug,
    level_info,
    level_warning,
    level_error,
    level_critical
} LogLevel;

void setLogLevel(LogLevel level);
void Log(LogLevel level, char *module, char *string, ...);