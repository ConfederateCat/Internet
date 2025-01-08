#pragma once

#include "Internet.hpp"

constexpr std::int32_t NTP_PORT = 123;
constexpr std::uint64_t NTP_PACKET_SIZE = 48;
constexpr std::uint32_t NTP_TIMESTAMP_DELAT = 2208988800;

constexpr std::uint8_t LI_NO_WARNING = 0b00;
constexpr std::uint8_t VN_3 = 0b011;
constexpr std::uint8_t MODE_CLIENT = 0b011;

constexpr std::uint8_t NTP_HEADER_BYTE = (LI_NO_WARNING << 6) | (VN_3 << 3) | MODE_CLIENT;

#pragma pack(push, 1)
typedef struct _NTP_3_HEADER
{
	std::uint8_t LeapIndicator : 2;					// 2 bits
	std::uint8_t VersionNumber : 3;					// 3 bits
	std::uint8_t Mode : 3;							// 3 bits
	std::uint8_t Stratum;							// 8 bits
	std::int8_t PollInterval;						// 8 bits
	std::int8_t Precision;							// 8 bits
	std::int32_t RootDelay;							// 32 bits
	std::int32_t RootDispersion;					// 32 bits
	std::uint32_t ReferenceClockIdentifier;			// 32 bits
	std::uint64_t ReferenceTimestamp;				// 64 bits
	std::uint64_t OriginateTimestamp;				// 64 bits
	std::uint64_t ReceiveTimestamp;					// 64 bits
	std::uint64_t TransmitTimestamp;				// 64 bit

} NTP_3_HEADER, *PNTP_3_HEADER;
#pragma pack(pop)