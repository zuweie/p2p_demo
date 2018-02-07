#ifndef  _D_LINKED_LIST_H_
#define  _D_LINKED_LIST_H_

#include "_entity.h"

#define LIST_HEAD(pl) (&((pl)->sentinel))
#define LIST_TAIL(pl) (&((pl)->sentinel))
#define LIST_FIRST(pl) LIST_HEAD(pl)->next
#define LIST_LAST(pl)  LIST_TAIL(pl)->prev
#define LIST_SIZE(pl) (pl->size)

typedef struct _ListNode {

  Entity _entity;
  struct _ListNode* prev;
  struct _ListNode* next;

} ListNode;

typedef struct _DLinkedList {

  ListNode sentinel;
  size_t  size;
  ecompare compare;
  
} DLinkedList;

extern int dlist_init(DLinkedList*, ecompare func);
extern int dlist_put(DLinkedList*, Entity);
extern int dlist_push(DLinkedList*, Entity);
extern ListNode* dlist_find(DLinkedList* plist, ListNode* start_from, Entity e, int(*filter)(Entity*e1, Entity*e2), Entity** _entity);
extern ListNode* dlist_remove(DLinkedList* plist, ListNode* start_from, Entity e, int (*filter)(Entity* e1, Entity* e2), Entity* _entity);
extern int dlist_remove_all(DLinkedList*, int(*cleanup)(Entity*));

#endif
