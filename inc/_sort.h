#ifndef _SORT_H_
#define _SORT_H_

#include "_entity.h"

extern int insertion_sort(Entity kv_arr[], size_t size, Ecmp compare);
extern int merge(Entity kv_arr[], size_t s1, size_t sm, size_t s2, Ecmp compare);
extern int merge_sort(Entity kv_arr[], size_t p, size_t r, Ecmp compare);
extern int quick_sort(Entity arr[], int p, int r, Ecmp compare, int (* partition_func)(Entity[], int, int, Ecmp));
extern int partition(Entity arr[], int p, int r, Ecmp compare);
extern int randomized_partition(Entity arr[], int p, int r, Ecmp);

#endif
