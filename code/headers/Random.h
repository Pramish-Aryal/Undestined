#include <random>
#include "utils.h"

class Random
{
	public:
	static types::i32 get_random(types::i32 lower, types::i32 upper)
	{
		static std::random_device rd;
		static std::mt19937 mt(rd());
		
		std::uniform_int_distribution<int> dist(lower, upper);
		
		return dist(mt);
	}
	
};