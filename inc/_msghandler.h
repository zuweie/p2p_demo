#ifndef _MSG_HANDLER_H_
#define _MSG_HANDLER_H_
#include "_rb_tree.h"
#include "_endpoint.h"

typedef RBTree Mhmap;
typedef int (*Handler) (Endpoint* endpoint, void* param);
typedef struct MsgHandler {
    int msg;
    Handler handler;
}MsgHandler;

extern int init_mhmap (Mhmap* map);
extern int add_handler (Mhmap* map, int msg,  Handler handler);
extern Handler find_handler (Mhmap* map, int msg);

#endif