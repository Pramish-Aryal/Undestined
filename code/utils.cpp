#include "utils.h"

using namespace types;
r32 Math::Q_rsqrt(r32 number )
{
    i32 i;
    r32 x2, y;
    const r32 threehalfs = 1.5f;
    x2 = number * 0.5f;
    y  = number;
    i  = *(i32*) &y;
    i  = 0x5f3759df - ( i >> 1 );
    y  = *(r32*)&i;
    y  = y * (threehalfs - (x2 * y * y));   // 1st iteration
	//  y  = y * ( threehalfs - ( x2 * y * y ) );   // 2nd iteration, this can be removed
    return y;
}

r32 Math::fast_sqrt(r32 number)
{
	return Q_rsqrt(number) * number;
}
bool contain(std::vector<std::string> holder, std::string checker){
	for(int i = 0; i < holder.size();i++)
		if(holder[i] == checker) return true;
	return false;
}