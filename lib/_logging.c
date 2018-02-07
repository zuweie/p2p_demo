#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <time.h>
#include <sys/timeb.h>
#include "_logging.h"

static LogLevel CURRENT_LEVEL = INFO;

extern void log_setlevel(LogLevel level) 
{
    CURRENT_LEVEL = level;
}

extern LogLevel log_getlevel() 
{
    return CURRENT_LEVEL;
}

extern const char* levelstr(LogLevel level) 
{
    switch (level){
        case DEBUG:
            return "DEG";
        break;
        case INFO:
            return "INF";
        break;
        case WARN:
            return "WAR";
        break;
        case ERROR:
            return "ERR";
        break;
        default:
            return "???";
    }
}

void log_msg (LogLevel level, const char* msg, ...) 
{
    if (level < CURRENT_LEVEL) return;
    va_list ap;
    // va_start 和 va_end 用于获取可变参数
    va_start(ap, msg);
    char fmt[LOGBUF] = {0};
    char timestamp[26];
    struct timeb tp;
    ftime(&tp);
    struct tm *st = localtime(&tp.time);
    sprintf(timestamp, "%02d:%02d:%02d.%03d",
            st->tm_hour, st->tm_min, st->tm_sec, tp.millitm);
    sprintf(fmt, "%s %s: ", timestamp, levelstr(level));
    // strcat 将两个字符串合并。
    strcat(fmt, msg);
    strcat(fmt, "\n");
    // vprintf 是一种函数，功能是送格式化输出到stdout中。
    vprintf(fmt, ap);
    va_end(ap);

}
