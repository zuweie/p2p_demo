#ifndef _PEER_MANAGER_H_
#define _PEER_MANAGER_H_

#include "_d_linked_list.h"
#include "_entity.h"

extern int peer_filter (Entity * e1, Entity * e2);
extern int peer_clean (Entity * e);
extern int init_peerlist (DLinkedList* list);
extern int add_peer (const char* id, Endpoint io);
extern Peer* find_peer(char* id);
extern int delete_peer(char* id);
extern void clean_peer_list();

#endif