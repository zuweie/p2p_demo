#ifndef _LOGGING_H_
#define _LOGGING_H_

#define LOGBUF 1024
#define log_debug(msg, args...) log_msg(DEBUG, msg, ##args);
#define log_info(msg, args...) log_msg(INFO, msg, ##args);
#define log_warn(msg, args...) log_msg(WARN, msg, ##args);
#define log_err(msg, args...) log_msg(ERROR, msg, ##args);

typedef enum _LogLevel {
    DEBUG = 0,
    INFO,
    WARN,
    ERROR
}LogLevel;

extern const char* levelstr(LogLevel level);
extern void log_msg(LogLevel level, const char* msg, ...);
extern void log_setlevel(LogLevel level);
extern LogLevel log_getlevel();
#endif