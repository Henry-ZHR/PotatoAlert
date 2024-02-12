#include "Core/Process.hpp"

#include <cstdint>
#include <cstdlib>
#include <string>


namespace c = PotatoAlert::Core;

void c::ExitCurrentProcess(uint32_t code)
{
	std::exit(code);
}

void c::ExitCurrentProcessWithError(uint32_t code)
{
	std::exit(code);
}
