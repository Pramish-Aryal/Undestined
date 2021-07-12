#include "utils.h"

types::r32 Math::Q_rsqrt( types::r32 number )
{
    types::i32 i;
    types::r32 x2, y;
    const types::r32 threehalfs = 1.5F;
    x2 = number * 0.5F;
    y  = number;
    i  = * ( types::i32 * ) &y;
    i  = 0x5f3759df - ( i >> 1 );
    y  = * ( types::r32 * ) &i;
    y  = y * ( threehalfs - ( x2 * y * y ) );   // 1st iteration
//  y  = y * ( threehalfs - ( x2 * y * y ) );   // 2nd iteration, this can be removed
    return y;
}

types::r32 Math::fast_sqrt(types::r32 number)
{
	return Q_rsqrt(number) * number;
}