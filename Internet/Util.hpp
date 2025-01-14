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

	float
	GetFrequency(
		void
	);

	char*
	GetIp(
		const char* Domain
	);

	void
	PrintBytes(
		void* Address,
		std::size_t Length,
		bool PrintLine
	);
}