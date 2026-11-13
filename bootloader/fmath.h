#ifndef FMATH_FLAG
#define FMATH_FLAG //Guard
#include "boot.h"

// Unsigned floats
typedef union {
    UINT16 Raw;
    struct {
        UINT16 Exponent : 5;
        UINT16 Mantissa : 11;
    } Parts;
} UFLOAT_16;

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

typedef union {
    UINT64 Raw[2];
    struct {
        UINT64 Exponent     : 15;
        UINT64 TopMantissa  : 49; // 64-15 = 49
        UINT64 BotMantissa;
    } Parts;
} UFLOAT_128;

typedef union {
    UINT64 Raw[4];
    struct {
        UINT64 Exponent     : 19;
        UINT64 TopMantissa  : 45; // 64-19 = 45
        UINT64 HighMantissa;
        UINT64 LowMantissa;
        UINT64 BotMantissa;
    } Parts;
} UFLOAT_256;


// Signed floats
typedef union {
    UINT16 Raw;
    struct {
        UINT16 Exponent : 5;
        UINT16 Mantissa : 10;
        UINT16 Sign     : 1;
    } Parts;
} FLOAT_16;

typedef union {
    UINT32 Raw;
    struct {
        UINT32 Exponent : 8;
        UINT32 Mantissa : 23;
        UINT32 Sign     : 1;
    } Parts;
} FLOAT_32;

typedef union {
    UINT64 Raw;
    struct {
        UINT64 Exponent : 11;
        UINT64 Mantissa : 53;
        UINT64 Sign     : 1;
    } Parts;
} FLOAT_64;

typedef union {
    UINT64 Raw[2];
    struct {
        UINT64 Exponent     : 15;
        UINT64 TopMantissa  : 49; // 64-15 = 49
        UINT64 BotMantissa  : 63;
        UINT64 Sign         : 1;
    } Parts;
} FLOAT_128;

typedef union {
    UINT64 Raw[4];
    struct {
        UINT64 Exponent     : 19;
        UINT64 TopMantissa  : 45; // 64-19 = 45
        UINT64 HighMantissa;
        UINT64 LowMantissa;
        UINT64 BotMantissa  : 63;
        UINT64 Sign         : 1;
    } Parts;
} FLOAT_256;

//#define INFINITY ((UFLOAT_256) - 1)
//#define NaN      ()
#endif