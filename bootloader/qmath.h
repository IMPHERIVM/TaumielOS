#ifndef QMATH_FLAG
#define QMATH_FLAG
#include "boot.h"


#define QMATH_SUCCESS 0

#define QMATH_ERROR(Status) ((UINTN) (MAX_BIT | (Status)))


//Errors
//Representation Category
#define QMATH_SIZE_ERROR                QMATH_ERROR(400)
#define QMATH_INVALID_FORMAT            QMATH_ERROR(401)
//Arguments Category
#define QMATH_INVALID_OPERAND           QMATH_ERROR(410)
//Aritmethic Category
#define QMATH_OPERATION_OVERFLOW        QMATH_ERROR(420)
#define QMATH_DIVISION_BY_ZERO          QMATH_ERROR(421)
//Operations Category
#define QMATH_UNSUPPORTED_OPERATION     QMATH_ERROR(430)
//Internal Category
#define QMATH_INTERNAL_ERROR            QMATH_ERROR(440)
#define QMATH_INVALID_STATE             QMATH_ERROR(441)


typedef union { //Número racional, Q16.16
    UINT32 Raw;
    struct {
        UINT16 FracPart;
        UINT16 IntPart;
    } Parts;
} Q16_16;

typedef union { //Número racional, Q32.32
    UINT64 Raw;
    struct {
        UINT32 FracPart;
        UINT32 IntPart;
    } Parts;
} Q32_32;

typedef union { //Número racional, Q64.64
    UINT64 Raw[2];
    struct {
        UINT64 FracPart;
        UINT64 IntPart;
    } Parts;
} Q64_64;
typedef union { //Número racional, Q128.128
    UINT64 Raw[4];
    struct {
        UINT64 BottomFracPart;
        UINT64 TopFracPart;
        UINT64 BottomIntPart;
        UINT64 TopIntPart;
    } Parts;
} Q128_128;

//Get the masks for the QFormat numbers
#define QMATH_FRAC_MASK16       ((UINT32)0x0000FFFFu) //defined for Q16.16
#define QMATH_INT_MASK16        ((UINT32)0xFFFF0000u)
#define QMATH_FRAC_MASK32       ((UINT64)0x00000000FFFFFFFFULL) //defined for Q32.32
#define QMATH_INT_MASK32        ((UINT64)0xFFFFFFFF00000000ULL)


#define QMATH_GET_FRAC_PART16(Number)   ((UINT16)(Number) & QMATH_FRAC_MASK16)
#define QMATH_GET_INT_PART16(Number)    ((UINT16)(Number) >> 16)
#define QMATH_GET_FRAC_PART32(Number)   ((UINT32)(Number) & QMATH_FRAC_MASK32)
#define QMATH_GET_INT_PART32(Number)    ((UINT32)(Number) >> 32)
#define QMATH_GET_FRAC_PART64(Number)   ((Number).Parts.FracPart)
#define QMATH_GET_INT_PART64(Number)    ((Number).Parts.IntPart)
 
#endif