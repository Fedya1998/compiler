enum types {
#define DEF_TYPE(type, num)\
    type = num,

#include "types.h"
    TYPE_EMPTY
#undef DEF_TYPE
};
