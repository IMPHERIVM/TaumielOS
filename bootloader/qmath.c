#include "boot.h"
#include "qmath.h"


Q16_16 QMATH_SUM_16(Q16_16 self, Q16_16 other) {
    Q16_16 sum;
    if (self.Parts.Intpart + other.Parts.Intpart > MAX_UINT16) {
        return sum;
    }
    return sum;
}