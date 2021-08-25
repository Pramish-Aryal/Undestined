#pragma once

#include <iostream>
#include <cstdlib>

namespace Fatal
{
	static void fatal_error(const std::string& error)
	{
		std::cout << error << std::endl;
		exit(-1);
	}
}