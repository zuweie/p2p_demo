#include <string.h>
#include "_entity.h"

extern int base_compare (Entity* e1, Entity* e2){
    Vtype vtype = e1->_vtype;
    int ret;
    switch (vtype) {
        case EINT:
            if (e1->_data.ie == e2->_data.ie)
                return 0;
            else if (e1->_data.ie > e2->_data.ie) 
                return 1;
            else
                return -1;
        case EFLOAT:
            if (e1->_data.fe == e2->_data.fe)
                return 0;
            else if (e1->_data.fe > e2->_data.fe) 
                return 1;
            else
                return -1;
        case ESTRING:
            ret = strcmp(e1->_data.string, e2->_data.string);
            if (ret > 0) return 1;
            else if (ret < 0 ) return -1;
            else if (ret == 0) return 0;
            else return -2;
        case EPOINTER:
            return e1->_data.pointer == e2->_data.pointer ? 0: -2;
    }
}
extern Entity i2e(int v){
    Entity e;
    e._data.ie = v;
    e._vtype = EINT;
    return e;
}
extern Entity f2e(float v){
    Entity e;
    e._data.fe = v;
    e._vtype = EFLOAT;
    return e;
}
extern Entity s2e(char* v){
    Entity e;
    e._data.string = v;
    e._vtype = ESTRING;
    return e;
}

extern Entity p2e(void* v){
    Entity e;
    e._data.pointer = v;
    e._vtype = EPOINTER;
    return e;
}