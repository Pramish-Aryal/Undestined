#pragma once

#include <stdint.h>

namespace types
{
	typedef int8_t i8;
	typedef int16_t i16;
	typedef int32_t i32;
	typedef int64_t i64;
	
	typedef uint8_t u8;
	typedef uint16_t u16;
	typedef uint32_t u32;
	typedef uint64_t u64;
	
	typedef float r32;
	typedef double r64;
}


class Math
{
	//Fast inverse square root from Quake3:
	static types::r32 Q_rsqrt( types::r32 number);
	static types::r32 fast_sqrt(types::r32 number);
};

template <typename T>
struct Vec2Generic
{
	union
	{
		struct
		{
			T x, y;
		};
		struct
		{
			T w, h;
		};
	};

	Vec2Generic(T x = 0, T y = 0) : x(x), y(y) {}
	Vec2Generic(const Vec2Generic& vec) : x(vec.x), y(vec.y) {}

	T dot(const Vec2Generic& vec) 							{ return x * vec.x + y * vec.y; }
	T mag() 												{ return Math::fast_sqrt(x * x + y * y);	}
	Vec2Generic normal()									{ return Vec2Generic(-y, x);				}
	Vec2Generic operator+(const Vec2Generic& vec) const		{ return Vec2Generic(x + vec.x, y + vec.y);	}
	Vec2Generic operator-(const Vec2Generic& vec) const		{ return Vec2Generic(x - vec.x, y - vec.y);	}
	Vec2Generic operator*(const float& a) 		   const	{ return Vec2Generic(x * a, y * a);			}
	Vec2Generic operator/(const float& a) 		   const	{ return Vec2Generic(x / a, y / a);			}
};

typedef Vec2Generic<types::r32> Vec2f;
typedef Vec2Generic<types::i32> Vec2i;