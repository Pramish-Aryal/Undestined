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

#define MAX(a, b) (a > b ? a : b)
#define MIN(a, b) (a < b ? a : b)
#define ABS(a) ((a) > 0 ? (a) : (-a))
#define SIGNOF(a) ((a) > 0 ? 1 : a < 0 ? -1 : 0)

template<typename T>
void swap(T& a, T& b)
{
	T temp = a;
	a = b;
	b = temp;;
}

class Math
{
	public:
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
	
	T dot(const Vec2Generic& vec) 					 { return x * vec.x + y * vec.y;                         }
	T magnitude() 								     { return Math::fast_sqrt(x * x + y * y);                }
	Vec2Generic perpendicular()					    { return Vec2Generic(-y, x);			                }
	Vec2Generic normal()					           { T r = 1 / magnitude(); return Vec2Generic(x*r, y*r);  }
	Vec2Generic operator+(const Vec2Generic& vec) const{ return Vec2Generic(x + vec.x, y + vec.y);             }
	Vec2Generic operator-(const Vec2Generic& vec) const{ return Vec2Generic(x - vec.x, y - vec.y);             }
	Vec2Generic operator*(const T& a) 	 	   const{ return Vec2Generic(x * a, y * a);		             }
	Vec2Generic operator*(const Vec2Generic& vec) const{ return Vec2Generic(x * vec.x, y * vec.y);             }
	Vec2Generic operator/(const T& a) 	 	   const{ return Vec2Generic(x / a, y / a);		             }
	Vec2Generic operator/(const Vec2Generic& vec) const{ return Vec2Generic(x / vec.x, y / vec.y);             }
	Vec2Generic operator+=(const Vec2Generic& vec)     { x += vec.x; y += vec.y; return *this;                 }
};

template<typename T>
inline Vec2Generic<T> operator/(const types::r32& fl, const Vec2Generic<T>& vec){ return Vec2Generic<T> ((T)(fl / (types::r64)vec.x),(T)(fl / (types::r64)vec.y)); }

template<typename T>
inline Vec2Generic<T> operator*(const types::r32& fl, const Vec2Generic<T>& vec){ return Vec2Generic<T> ((T)(fl * (types::r32)vec.x),(T)(fl * (types::r32)vec.y)); }

typedef Vec2Generic<types::r32> Vec2f;
typedef Vec2Generic<types::i32> Vec2i;