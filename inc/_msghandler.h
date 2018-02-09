#ifndef _MSG_HANDLER_H_
#define _MSG_HANDLER_H_
#include "_rb_tree.h"
typedef RBTree Mhmap;
typedef int (*Handler) (void* param);
typedef struct MsgHandler {
    int msg;
    Handler handler;
}MsgHandler;

#endif