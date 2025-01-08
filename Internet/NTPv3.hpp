#pragma once

#include "Internet.hpp"

constexpr std::int32_t NTP_PORT = 123;
constexpr std::uint64_t NTP_PACKET_SIZE = 48;
constexpr std::uint32_t NTP_TIMESTAMP_DELAT = 2208988800;

constexpr std::uint8_t LI_NO_WARNING = 0b00;
constexpr std::uint8_t VN_3 = 0b011;
constexpr std::uint8_t MODE_CLIENT = 0b011;

#define NTP_CALCULATE_ATTRIBUTES(LeapIndicator, VersionNumber, Mode) \
	((LeapIndicator << 6) | (VersionNumber << 3) | Mode)


#pragma pack(push, 1)
typedef struct _NTP_3_HEADER
{
	union {
        struct {
            std::uint8_t LeapIndicator : 2;			// 2 bits
            std::uint8_t VersionNumber : 3;			// 3 bits
            std::uint8_t Mode : 3;					// 3 bits
        };
		std::uint8_t Attributes;					// 8 bits, can't set bits directly due to the compiler inverting the bits
    };
	std::uint8_t Stratum;							// 8 bits
	std::int8_t PollInterval;						// 8 bits
	std::int8_t Precision;							// 8 bits
	std::int32_t RootDelay;							// 32 bits
	std::int32_t RootDispersion;					// 32 bits
	std::uint32_t ReferenceClockIdentifier;			// 32 bits
	struct											// 64 bits
	{
		std::uint32_t High;
		std::uint32_t Low;
	} ReferenceTimestamp;
	struct											// 64 bits
	{
		std::uint32_t High;
		std::uint32_t Low;
	} OriginateTimestamp;
	struct											// 64 bits
	{
		std::uint32_t High;
		std::uint32_t Low;
	} ReceiveTimestamp;
	struct											// 64 bits
	{
		std::uint32_t High;
		std::uint32_t Low;
	} TransmitTimestamp;
} NTP_3_HEADER, * PNTP_3_HEADER;
#pragma pack(pop)