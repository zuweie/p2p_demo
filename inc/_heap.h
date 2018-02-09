#ifndef _HEAD_H_
#define _HEAD_H_

#include "_entity.h"

#define PARENT(i) ((i-1)>>1))
#define LEFT(i) ((i<<1) + 0x1)
#define RIGHT(i) ((i<<1) + 0x2)

extern int max_heapify(Entity arr[], int i, size_t heap_sz, Ecmp compare);
extern int build_max_heap(Entity arr[], size_t size, Ecmp compare);
extern int heap_sort(Entity arr[], size_t size, Ecmp compare);

#endif
