#ifndef QMATH_FLAG
#define QMATH_FLAG
#include "boot.h"

typedef UINTN QMATH_STATUS;
#define QMATH_SUCCESS ((QMATH_STATUS) 0)

#define QMATH_ERROR(Status) ((UINTN) (MAX_BIT | (Status)))


//Errors
//Representation Category
#define QMATH_SIZE_ERROR                    QMATH_ERROR(400)
#define QMATH_INVALID_FORMAT                QMATH_ERROR(401)
#define QMATH_CONVERSION_ERROR              QMATH_ERROR(402)
#define QMATH_OUT_OF_RANGE                  QMATH_ERROR(403)
//Arguments Category
#define QMATH_INVALID_OPERAND               QMATH_ERROR(410)
//#define QMATH_TOO_MANY_OPERANDS             QMATH_ERROR(411)
//#define QMATH_MISSING_ARGUMENT              QMATH_ERROR(412)
//#define QMATH_INVALID_ARGUMENT_TYPE         QMATH_ERROR(413)
#define QMATH_NULL_POINTER                  QMATH_ERROR(414)
//Aritmethic Category
#define QMATH_OPERATION_OVERFLOW            QMATH_ERROR(420)
#define QMATH_DIVISION_BY_ZERO              QMATH_ERROR(421)
#define QMATH_OPERATION_UNDERFLOW           QMATH_ERROR(422)
#define QMATH_NEGATIVE_RESULT               QMATH_ERROR(423)
//Operations Category
#define QMATH_UNSUPPORTED_OPERATION         QMATH_ERROR(430)
//Internal Category
#define QMATH_INTERNAL_ERROR                QMATH_ERROR(440)
#define QMATH_INVALID_STATE                 QMATH_ERROR(441)


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

typedef union { //número racional, Q256.256 .De momento, el tipo de dato más grande, y posiblemente el último
    UINT64 Raw[8];
    struct {
        UINT64 BottomFracPart;
        UINT64 LowMedFracPart;
        UINT64 HighMedFracPart;
        UINT64 TopFracPart;
        UINT64 BottomIntPart;
        UINT64 LowMedIntPart;
        UINT64 HighMedIntPart;
        UINT64 TopIntPart;
    } Parts;
} Q256_256;



typedef union { //Signed Q16.16
    UINT32 Raw;
    struct {
        UINT16  FracPart;
        UINT16  IntPart: 15;
        UINT16  Sign: 1;
    } Parts;
} SQ16_16;

typedef union {
    UINT64 Raw;
    struct {
        UINT32  FracPart;
        UINT32  IntPart: 31;
        UINT32  Sign: 1;
    } Parts;
} SQ32_32;

typedef union {
    UINT64 Raw[2];
    struct {
        UINT64  FracPart;
        UINT64  IntPart: 63;
        UINT64  Sign: 1;
    } Parts;
} SQ64_64;

typedef union {
    UINT64 Raw[4];
    struct {
        UINT64  BottomFracPart;
        UINT64  TopFracPart;
        UINT64  BottomIntPart;
        UINT64  TopIntPart: 63;
        UINT64  Sign: 1;
    } Parts;
} SQ128_128;

typedef union {
    UINT64 Raw[8];
    struct {
        UINT64  BottomFracPart;
        UINT64  LowMedFracPart;
        UINT64  HighMedFracPart;
        UINT64  TopFracPart;
        UINT64  BottomIntPart;
        UINT64  LowMedIntPart;
        UINT64  HighMedIntPart;
        UINT64  TopIntPart: 63;
        UINT64  Sign: 1;
    } Parts;
} SQ256_256;





//Get the masks for the QFormat numbers
#define QMATH_FRAC_MASK16       ((UINT32)0x0000FFFFu) //defined for Q16.16
#define QMATH_INT_MASK16        ((UINT32)0xFFFF0000u)
#define QMATH_FRAC_MASK32       ((UINT64)0x00000000FFFFFFFFULL) //defined for Q32.32
#define QMATH_INT_MASK32        ((UINT64)0xFFFFFFFF00000000ULL)


#define QMATH_GET_FRAC_PART16(Number)   ((UINT32)(Number) & QMATH_FRAC_MASK16)
#define QMATH_GET_INT_PART16(Number)    ((UINT32)(Number) >> 16)
#define QMATH_GET_FRAC_PART32(Number)   ((UINT64)(Number) & QMATH_FRAC_MASK32)
#define QMATH_GET_INT_PART32(Number)    ((UINT64)(Number) >> 32)
#define QMATH_GET_FRAC_PART64(Number)   ((Number).Parts.FracPart)
#define QMATH_GET_INT_PART64(Number)    ((Number).Parts.IntPart)
 


//Operations
//Q16.16
QMATH_STATUS QMATH_SUM_16(Q16_16 A, Q16_16 B, Q16_16 *Result);
QMATH_STATUS QMATH_SUB_16(Q16_16 A, Q16_16 B, Q16_16 *Result);
QMATH_STATUS QMATH_MULT_16(Q16_16 A, Q16_16 B, Q16_16 *Result);
QMATH_STATUS QMATH_MULT_16_EX(Q16_16 A, Q16_16 B, Q32_32 *Result);
QMATH_STATUS QMATH_DIV_16(Q16_16 A, Q16_16 B, Q16_16 *Result);
QMATH_STATUS QMATH_SQ_16(Q16_16 A, Q16_16 *Result);
QMATH_STATUS QMATH_INV_16(Q16_16 A, Q16_16 *Result); // recibe un valor n, devuelve 1/n, de ser más barato que QMATH_DIV_16(1, n), debatir si implementar la segunda con esta
QMATH_STATUS QMATH_SQRT_16(Q16_16 A, Q16_16 *Result); //No sufro aquí de trabajar con raíces negativas.
QMATH_STATUS QMATH_ISQRT_16(Q16_16 A, Q16_16 *Result); //Raíz cuadrada truncada a enteros
QMATH_STATUS QMATH_RSQRT_16(Q16_16 A, Q16_16 *Result); //Necesario para gráficas y simulaciones más eficientes para normalización de vectores
QMATH_STATUS QMATH_RSQRT_16_EX(Q16_16 A, Q32_32 *Result);
QMATH_STATUS QMATH_GT_16(Q16_16 A, Q16_16 B, BOOLEAN *Result); // TRUE si A > B, FALSE si A <= B.
QMATH_STATUS QMATH_LT_16(Q16_16 A, Q16_16 B, BOOLEAN *Result);
QMATH_STATUS QMATH_GE_16(Q16_16 A, Q16_16 B, BOOLEAN *Result);
QMATH_STATUS QMATH_LE_16(Q16_16 A, Q16_16 B, BOOLEAN *Result);
QMATH_STATUS QMATH_EQ_16(Q16_16 A, Q16_16 B, BOOLEAN *Result);     // TRUE si A=B; FALSE otherwise
QMATH_STATUS QMATH_NE_16(Q16_16 A, Q16_16 B, BOOLEAN *Result);

//Q32.32

QMATH_STATUS QMATH_SUM_32(Q32_32 A, Q32_32 B, Q32_32 *Result);
QMATH_STATUS QMATH_SUB_32(Q32_32 A, Q32_32 B, Q32_32 *Result);
QMATH_STATUS QMATH_MULT_32(Q32_32 A, Q32_32 B, Q32_32 *Result);
QMATH_STATUS QMATH_MULT_32_EX(Q32_32 A, Q32_32 B, Q64_64 *Result);
QMATH_STATUS QMATH_DIV_32(Q32_32 A, Q32_32 B, Q32_32 *Result);
QMATH_STATUS QMATH_SQ_32(Q32_32 A, Q32_32 *Result);
QMATH_STATUS QMATH_INV_32(Q32_32 A, Q32_32 *Result); // recibe un valor n, devuelve 1/n, de ser más barato que QMATH_DIV_32(1, n), debatir si implementar la segunda con esta
QMATH_STATUS QMATH_SQRT_32(Q32_32 A, Q32_32 *Result); //No sufro aquí de trabajar con raíces negativas.
QMATH_STATUS QMATH_ISQRT_32(Q32_32 A, Q32_32 *Result); //Raíz cuadrada truncada a enteros
QMATH_STATUS QMATH_RSQRT_32(Q32_32 A, Q32_32 *Result); //Necesario para gráficas y simulaciones más eficientes para normalización de vectores
QMATH_STATUS QMATH_RSQRT_32_EX(Q32_32 A, Q64_64 *Result);
QMATH_STATUS QMATH_GT_32(Q32_32 A, Q32_32 B, BOOLEAN *Result); // TRUE si A > B, FALSE si A <= B.
QMATH_STATUS QMATH_LT_32(Q32_32 A, Q32_32 B, BOOLEAN *Result);
QMATH_STATUS QMATH_GE_32(Q32_32 A, Q32_32 B, BOOLEAN *Result);
QMATH_STATUS QMATH_LE_32(Q32_32 A, Q32_32 B, BOOLEAN *Result);
QMATH_STATUS QMATH_EQ_32(Q32_32 A, Q32_32 B, BOOLEAN *Result);     // TRUE si A=B; FALSE otherwise
QMATH_STATUS QMATH_NE_32(Q32_32 A, Q32_32 B, BOOLEAN *Result);

//Q64.64

QMATH_STATUS QMATH_SUM_64(Q64_64 A, Q64_64 B, Q64_64 *Result);
QMATH_STATUS QMATH_SUB_64(Q64_64 A, Q64_64 B, Q64_64 *Result);
QMATH_STATUS QMATH_MULT_64(Q64_64 A, Q64_64 B, Q64_64 *Result);
QMATH_STATUS QMATH_MULT_64_EX(Q64_64 A, Q64_64 B, Q128_128 *Result);
QMATH_STATUS QMATH_DIV_64(Q64_64 A, Q64_64 B, Q64_64 *Result);
QMATH_STATUS QMATH_SQ_64(Q64_64 A, Q64_64 *Result);
QMATH_STATUS QMATH_INV_64(Q64_64 A, Q64_64 *Result); // recibe un valor n, devuelve 1/n, de ser más barato que QMATH_DIV_32(1, n), debatir si implementar la segunda con esta
QMATH_STATUS QMATH_SQRT_64(Q64_64 A, Q64_64 *Result); //No sufro aquí de trabajar con raíces negativas.
QMATH_STATUS QMATH_ISQRT_64(Q64_64 A, Q64_64 *Result); //Raíz cuadrada truncada a enteros
QMATH_STATUS QMATH_RSQRT_64(Q64_64 A, Q64_64 *Result); //Necesario para gráficas y simulaciones más eficientes para normalización de vectores
QMATH_STATUS QMATH_RSQRT_64_EX(Q64_64 A, Q128_128 *Result);
QMATH_STATUS QMATH_GT_64(Q64_64 A, Q64_64 B, BOOLEAN *Result); // TRUE si A > B, FALSE si A <= B.
QMATH_STATUS QMATH_LT_64(Q64_64 A, Q64_64 B, BOOLEAN *Result);
QMATH_STATUS QMATH_GE_64(Q64_64 A, Q64_64 B, BOOLEAN *Result);
QMATH_STATUS QMATH_LE_64(Q64_64 A, Q64_64 B, BOOLEAN *Result);
QMATH_STATUS QMATH_EQ_64(Q64_64 A, Q64_64 B, BOOLEAN *Result);     // TRUE si A=B; FALSE otherwise
QMATH_STATUS QMATH_NE_64(Q64_64 A, Q64_64 B, BOOLEAN *Result);

// SQ16.16

QMATH_STATUS QMATH_SUM_S16(SQ16_16 A, SQ16_16 B, SQ16_16 *Result);
QMATH_STATUS QMATH_SUB_S16(SQ16_16 A, SQ16_16 B, SQ16_16 *Result);
QMATH_STATUS QMATH_MULT_S16(SQ16_16 A, SQ16_16 B, SQ16_16 *Result); 
QMATH_STATUS QMATH_MULT_S16_EX(SQ16_16 A, SQ16_16 B, SQ32_32 *Result);
QMATH_STATUS QMATH_DIV_S16(SQ16_16 A, SQ16_16 B, SQ16_16 *Result);
QMATH_STATUS QMATH_SQ_S16(SQ16_16 A, SQ16_16 *Result);
QMATH_STATUS QMATH_INV_S16(SQ16_16 A, SQ16_16 *Result); // recibe un valor n, devuelve 1/n, de ser más barato que QMATH_DIV_16(1, n), debatir si implementar la segunda con esta
QMATH_STATUS QMATH_SQRT_S16(SQ16_16 A, SQ16_16 *Result); //Habrá que tratar con raíces negativas.
QMATH_STATUS QMATH_ISQRT_S16(SQ16_16 A, SQ16_16 *Result); //Raíz cuadrada truncada a enteros
QMATH_STATUS QMATH_RSQRT_S16(SQ16_16 A, SQ16_16 *Result); //Necesario para gráficas y simulaciones más eficientes para normalización de vectores
QMATH_STATUS QMATH_RSQRT_S16_EX(SQ16_16 A, SQ32_32 *Result);
QMATH_STATUS QMATH_GT_S16(SQ16_16 A, SQ16_16 B, BOOLEAN *Result); // TRUE si A > B, FALSE si A <= B.
QMATH_STATUS QMATH_LT_S16(SQ16_16 A, SQ16_16 B, BOOLEAN *Result);
QMATH_STATUS QMATH_GE_S16(SQ16_16 A, SQ16_16 B, BOOLEAN *Result);
QMATH_STATUS QMATH_LE_S16(SQ16_16 A, SQ16_16 B, BOOLEAN *Result);
QMATH_STATUS QMATH_EQ_S16(SQ16_16 A, SQ16_16 B, BOOLEAN *Result);     // TRUE si A=B; FALSE otherwise
QMATH_STATUS QMATH_NE_S16(SQ16_16 A, SQ16_16 B, BOOLEAN *Result);

// SQ32.32

QMATH_STATUS QMATH_SUM_S32(SQ32_32 A, SQ32_32 B, SQ32_32 *Result);
QMATH_STATUS QMATH_SUB_S32(SQ32_32 A, SQ32_32 B, SQ32_32 *Result);
QMATH_STATUS QMATH_MULT_S32(SQ32_32 A, SQ32_32 B, SQ32_32 *Result); 
QMATH_STATUS QMATH_MULT_S32_EX(SQ32_32 A, SQ32_32 B, SQ64_64 *Result);
QMATH_STATUS QMATH_DIV_S32(SQ32_32 A, SQ32_32 B, SQ32_32 *Result);
QMATH_STATUS QMATH_SQ_S32(SQ32_32 A, SQ32_32 *Result);
QMATH_STATUS QMATH_INV_S32(SQ32_32 A, SQ32_32 *Result); // recibe un valor n, devuelve 1/n, de ser más barato que QMATH_DIV_16(1, n), debatir si implementar la segunda con esta
QMATH_STATUS QMATH_SQRT_S32(SQ32_32 A, SQ32_32 *Result); //Habrá que tratar con raíces negativas.
QMATH_STATUS QMATH_ISQRT_S32(SQ32_32 A, SQ32_32 *Result); //Raíz cuadrada truncada a enteros
QMATH_STATUS QMATH_RSQRT_S32(SQ32_32 A, SQ32_32 *Result); //Necesario para gráficas y simulaciones más eficientes para normalización de vectores
QMATH_STATUS QMATH_RSQRT_S32_EX(SQ32_32 A, SQ64_64 *Result);
QMATH_STATUS QMATH_GT_S32(SQ32_32 A, SQ32_32 B, BOOLEAN *Result); // TRUE si A > B, FALSE si A <= B.
QMATH_STATUS QMATH_LT_S32(SQ32_32 A, SQ32_32 B, BOOLEAN *Result);
QMATH_STATUS QMATH_GE_S32(SQ32_32 A, SQ32_32 B, BOOLEAN *Result);
QMATH_STATUS QMATH_LE_S32(SQ32_32 A, SQ32_32 B, BOOLEAN *Result);
QMATH_STATUS QMATH_EQ_S32(SQ32_32 A, SQ32_32 B, BOOLEAN *Result);     // TRUE si A=B; FALSE otherwise
QMATH_STATUS QMATH_NE_S32(SQ32_32 A, SQ32_32 B, BOOLEAN *Result);

//Conversions:
QMATH_STATUS QMATH_16_TO_32_CONV(Q16_16 A, Q32_32 *Result);
QMATH_STATUS QMATH_16_TO_64_CONV(Q16_16 A, Q64_64 *Result);
QMATH_STATUS QMATH_16_TO_128_CONV(Q16_16 A, Q128_128 *Result);
QMATH_STATUS QMATH_16_TO_256_CONV(Q16_16 A, Q256_256 *Result);
QMATH_STATUS QMATH_32_TO_64_CONV(Q32_32 A, Q64_64 *Result);
QMATH_STATUS QMATH_32_TO_128_CONV(Q32_32 A, Q128_128 *Result);
QMATH_STATUS QMATH_32_TO_256_CONV(Q32_32 A, Q256_256 *Result);
QMATH_STATUS QMATH_64_TO_128_CONV(Q64_64 A, Q128_128 *Result);
QMATH_STATUS QMATH_64_TO_256_CONV(Q64_64 A, Q256_256 *Result);
QMATH_STATUS QMATH_128_TO_256_CONV(Q128_128 A, Q256_256 *Result);

QMATH_STATUS QMATH_S16_TO_S32_CONV(SQ16_16 A, SQ32_32 *Result);
QMATH_STATUS QMATH_S16_TO_S64_CONV(SQ16_16 A, SQ64_64 *Result);
QMATH_STATUS QMATH_S16_TO_S128_CONV(SQ16_16 A, SQ128_128 *Result);
QMATH_STATUS QMATH_S16_TO_S256_CONV(SQ16_16 A, SQ256_256 *Result);
QMATH_STATUS QMATH_S32_TO_S64_CONV(SQ32_32 A, SQ64_64 *Result);
QMATH_STATUS QMATH_S32_TO_S128_CONV(SQ32_32 A, SQ128_128 *Result);
QMATH_STATUS QMATH_S32_TO_S256_CONV(SQ32_32 A, SQ256_256 *Result);
QMATH_STATUS QMATH_S64_TO_S128_CONV(SQ64_64 A, SQ128_128 *Result);
QMATH_STATUS QMATH_S64_TO_S256_CONV(SQ64_64 A, SQ256_256 *Result);
QMATH_STATUS QMATH_S128_TO_S256_CONV(SQ128_128 A, SQ256_256 *Result);

//Inverse Conversions



//Helpers de Promoción
static inline Q32_32 QMATH_16_TO_32(Q16_16 A)       { Q32_32 Result; (VOID)QMATH_16_TO_32_CONV(A, &Result); return Result;}
static inline Q64_64 QMATH_16_TO_64(Q16_16 A)       { Q64_64 Result; (VOID)QMATH_16_TO_64_CONV(A, &Result); return Result;}
static inline Q128_128 QMATH_16_TO_128(Q16_16 A)    { Q128_128 Result; (VOID)QMATH_16_TO_128_CONV(A, &Result); return Result;}
static inline Q256_256 QMATH_16_TO_256(Q16_16 A)    { Q256_256 Result; (VOID)QMATH_16_TO_256_CONV(A, &Result); return Result;}
static inline Q64_64 QMATH_32_TO_64(Q32_32 A)       { Q64_64 Result; (VOID)QMATH_32_TO_64_CONV(A, &Result); return Result;}
static inline Q128_128 QMATH_32_TO_128(Q32_32 A)    { Q128_128 Result; (VOID)QMATH_32_TO_128_CONV(A, &Result); return Result;}
static inline Q256_256 QMATH_32_TO_256(Q32_32 A)    { Q256_256 Result; (VOID)QMATH_32_TO_256_CONV(A, &Result); return Result;}
static inline Q128_128 QMATH_64_TO_128(Q64_64 A)    { Q128_128 Result; (VOID)QMATH_64_TO_128_CONV(A, &Result); return Result;}
static inline Q256_256 QMATH_64_TO_256(Q64_64 A)    { Q256_256 Result; (VOID)QMATH_64_TO_256_CONV(A, &Result); return Result;}
static inline Q256_256 QMATH_128_TO_256(Q128_128 A) { Q256_256 Result; (VOID)QMATH_128_TO_256_CONV(A, &Result); return Result;}


//Size confirmations:
_Static_assert(sizeof(Q16_16) == 4, "Invalid Q16_16 size");
_Static_assert(sizeof(SQ16_16) == 4, "Invalid SQ16_16 size");
_Static_assert(sizeof(Q32_32) == 8, "Invalid Q32_32 size");
_Static_assert(sizeof(SQ32_32) == 8, "Invalid SQ32_32 size");
_Static_assert(sizeof(Q64_64) == 16, "Invalid Q64_64 size");
_Static_assert(sizeof(SQ64_64) == 16, "Invalid SQ64_64 size");
_Static_assert(sizeof(Q128_128) == 32, "Invalid Q128_128 size");
_Static_assert(sizeof(SQ128_128) == 32, "Invalid SQ128_128 size");
_Static_assert(sizeof(Q256_256) == 64, "Invalid Q256_256 size");
_Static_assert(sizeof(SQ256_256) == 64, "Invalid SQ256_256 size");


#endif