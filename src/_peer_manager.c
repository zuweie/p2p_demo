#include <stdlib.h>
#include <string.h>
#include "_peer_manager.h"
#include "_d_linked_list.h"
#include "_peer.h"

// 只在此文件访问，相当于私有类变量

static DLinkedList g_peer_list;

static int peer_filter (Entity * e1, Entity * e2) 
{
    if (e2->_vtype != ESTRING) {
        return 0;
    }
    Peer * p1 = (Peer*)(e1->_data.pointer);
    return strcmp(p1->id, e2->_data.string) == 0;
}

static int peer_clean (Entity * e) 
{
    if (e->_data.pointer) {
        free(e->_data.pointer);
    }
    return 0;
} 

extern int init_peerlist () 
{
    dlist_init(&g_peer_list);
}

extern int update_peer (const char* id, Endpoint* io) 
{

    Peer* peer = find_peer(id);
    if (peer) {
        // update the io.
        peer->io = *io;
    }else{
        
        peer = (Peer*)malloc(sizeof(Peer));

        strcpy(peer->id, id);
        peer->io = *io;
        Entity e = p2e((void*)peer);
        dlist_push(&g_peer_list, e);
    }
    return 0;
}

extern Peer* find_peer(char* id) 
{
    Entity *pe;
    dlist_find(&g_peer_list, LIST_FIRST(&g_peer_list), s2e(id), peer_filter, &pe);
    if (pe){
        return (Peer*)(pe->_data.pointer);
    }
    return NULL;
}

extern int delete_peer(char* id) 
{
    Entity entity;
    entity._data.pointer = NULL;
    dlist_remove(&g_peer_list, LIST_FIRST(&g_peer_list), s2e(id), peer_filter, &entity);
    if (entity._data.pointer) {
        free(entity._data.pointer);
        return 0;
    }
    return -1;
}

extern void clean_peer_list() 
{
    dlist_remove_all(&g_peer_list, peer_clean);
}

extern int get_all_peer(Peer **peers) 
{
    ListNode * pf = LIST_FIRST(&g_peer_list); 
    int i=0;
    for (;pf != LIST_TAIL(&g_peer_list); pf = pf->next, ++i){
        peers[i] = (Peer*)(pf->_entity._data.pointer);
    }
    return 0;
}

extern int peer_size()
{
    return (int)g_peer_list.size;
}