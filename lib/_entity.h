#ifndef _UDATA_H_
#define _UDATA_H_

// 对所有数据的一种抽象。

typedef enum Vtype { EINT, EFLOAT, ESTRING, EPOINTER} Vtype;

typedef union _Udata {
    int ie;
    float fe;
    char* string;
    void* pointer;
} Udata;

typedef struct _Entity {
    Udata _data;
    Vtype _vtype;
}Entity;

typedef int (*ecompare)(Entity* e1, Entity* e2);
extern int base_compare (Entity* e1, Entity* e2);

extern Entity i2e(int);
extern Entity f2e(float);
extern Entity s2e(char*);
extern Entity p2e(void*);
#endif

