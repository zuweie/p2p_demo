#ifndef _PEER_MANAGER_H_
#define _PEER_MANAGER_H_

#include "_d_linked_list.h"
#include "_entity.h"
#include "_endpoint.h"
#include "_peer.h"

extern int init_peerlist ();
extern int update_peer (const char* id, Endpoint* io);
extern Peer* find_peer(char* id);
extern int delete_peer(char* id);
extern void clean_peer_list();
extern void all_peer(Peer**);
#endif