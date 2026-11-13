#include "boot.h"
#include "qmath.h"


QMATH_STATUS QMATH_SUM_16(Q16_16 self, Q16_16 other, Q16_16 *Result) {
    if (self.Parts.Intpart + other.Parts.Intpart > MAX_UINT16) {
        return QMATH_OPERATION_OVERFLOW;
    }
    return sum;
}