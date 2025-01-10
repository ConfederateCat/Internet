#pragma once

#include "Client.hpp"

namespace Util
{
	std::uint16_t
	SwitchEndianness16(
		std::uint16_t Value
	);

	std::uint32_t
	SwitchEndianness32(
		std::uint32_t Value
	);

	std::uint32_t
	SwitchEndianness64(
	std::uint64_t Value
	);
}