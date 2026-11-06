#ifndef QMATH_FLAG
#define QMATH_FLAG
#include "boot.h"


#define QMATH_SUCCESS RETURN_SUCCESS

#define QMATH_ERROR(Status) ((UINTN) (MAX_BIT | (Status)))

#define QMATH_OPERATION_OVERFLOW        QMATH_ERROR(400)
#define QMATH_INVALID_OPERAND           QMATH_ERROR(401)
#define QMATH_DIVISION_BY_ZERO          QMATH_ERROR(402)
#define QMATH_UNSUPPORTED_OPERATION     QMATH_ERROR(403)


typedef union { //Número racional, Q16.16
    UINT32 Raw;
    struct {
        UINT32 DecPart : 16;
        UINT32 IntPart : 16;
    } Parts;
} Q16_16;

typedef union { //Número racional, Q32.32
    UINT64 Raw;
    struct {
        UINT32 DecPart : 32;
        UINT32 IntPart : 32;
    } Parts;
} Q32_32;

typedef union { //Número racional, Q64.64
    UINT64 Raw[2];
    struct {
        UINT64 DecPart;
        UINT64 IntPart;
    } Parts;
} Q64_64;

//Get the masks for the QFormat numbers
#define QMATH_DEC_MASK16(Number) ((UINT32)Number & (1-MAX_UINT16)) //defined for Q16.16
#define QMATH_DEC_MASK32(Number) ((UINT64)Number & (1-MAX_UINT32)) //defined for Q32.32
#define QMATH_DEC_MASK64(Number) ((Q64_64)Number.Parts.DecPart)    //defined for Q64.64

 
#endif