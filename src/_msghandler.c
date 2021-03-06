#include "_msghandler.h"
#include "_entity.h"
#include "_logging.h"

#define MSG_CMP(x,y) do{\
    if ((x) < (y)) return -1;\
    else if ((x) > (y)) return 1;\
    else return  0; } while(0)

static int mh_compare (Entity* e1, Entity* e2) 
{
    MsgHandler *h1 = (MsgHandler*)(e1->_data.pointer);
    MsgHandler *h2 = (MsgHandler*)(e2->_data.pointer);
    MSG_CMP(h1->msg, h2->msg);
}

static int mh_filter(Entity *e1, Entity* e2) 
{
    MsgHandler *h1 = (MsgHandler*)(e1->_data.pointer);
    MSG_CMP(h1->msg, e2->_data.ie);
}

extern int init_mhmap (Mhmap* map)
{
    init_rbtree(map, &mh_compare);
}

extern int add_handler (Mhmap* map, int msg,  Handler handler) 
{
    log_info("add_handler");
    size_t mh_size = sizeof(MsgHandler);
    MsgHandler* mh = (MsgHandler*)malloc(mh_size);
    mh->msg = msg;
    mh->handler = handler;
    
    rb_insert(map, p2e(mh));
}

extern Handler find_handler (Mhmap* map, int msg) 
{
    extern RBTreeNode* rb_search(RBTree* prb, RBTreeNode* pt, Entity e, Ecmp filter, Entity** entity);

    Entity *entity;
    rb_search(map, map->root, i2e(msg), mh_filter, &entity);
    if (entity) {
        return ((MsgHandler*) (entity->_data.pointer))->handler;
    }
    return NULL;
} 