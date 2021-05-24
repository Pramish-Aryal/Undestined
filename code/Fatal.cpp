#include "Fatal.h"

namespace Fatal
{
	void fatal_error(std::string error)
	{
		std::cout << error << std::endl;
		exit(-1);
	}
}