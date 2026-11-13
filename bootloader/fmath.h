#ifndef FMATH_FLAG
#define FMATH_FLAG //Guard
#include "boot.h"



typedef union {
    UINT32 Raw;
    struct {
        UINT32 Exponent : 8;
        UINT32 Mantissa : 24;
    } Parts;
} UFLOAT_32;
typedef union {
    UINT64 Raw;
    struct {
        UINT64 Exponent : 11;
        UINT64 Mantissa : 53;
    } Parts;
} UFLOAT_64;



#endif