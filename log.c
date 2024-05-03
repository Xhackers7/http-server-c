#include "log.h"
#include <stdio.h>
#include <stdlib.h>


static const char *const g_debug_colorCodes[] = {
    [level_debug] = "\033[2;37mDEBUG: ",
    [level_info] = "\033[0;35m",
    [level_warning] = "\033[1;93mWARN! ",
    [level_error] = "\033[1;31mERR!! ",
    [level_critical] = "\033[1;97;101mABORTED: ",
};

static LogLevel g_currentLevel = level_info;

void setLogLevel(LogLevel level)
{
    g_currentLevel = level;
}

void Log(LogLevel level, char *module, char *string, ...)
{
    va_list args;
    va_start(args, string);
    if (level < g_currentLevel)
        return;
    printf("%s[%s] ",g_debug_colorCodes[level], module);
    vprintf(string, args);
    va_end(args);
    printf("\033[0m\n");
    if (level == level_critical) exit(1);
}
