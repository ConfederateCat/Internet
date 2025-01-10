#pragma once

#include "Client.hpp"

//
// Standard NTP port, assigned by the Internet Assigned Numbers Authority.
//
constexpr std::int32_t NTP_PORT = 123;

//
// Difference in seconds between the Unix epoch (January 1, 1970, 00:00:00 UTC) and the NTP epoch (January 1, 1900, 00:00:00 UTC).
//
constexpr std::uint32_t NTP_TIMESTAMP_DELTA = 2208988800;

//
// All following data taken from https://datatracker.ietf.org/doc/html/rfc1305#appendix-A.
// To-Do: Add values for other members of the struct.
// 
// Attributes LeadIndicator.
//
constexpr std::uint8_t LI_NO_WARNING = 0b00;
constexpr std::uint8_t LI_LAST_MINUTE_61 = 0b01;
constexpr std::uint8_t LI_LAST_MINUTE_59 = 0b10;
constexpr std::uint8_t LI_ALARM_CONDITION = 0b11;

//
// Attributes VersionNumber.
//
constexpr std::uint8_t VN_3 = 0b011;

//
// Attributes Mode.
//
constexpr std::uint8_t MODE_RESERVED = 0b000;
constexpr std::uint8_t MODE_SYMMETRIC_ACTIVE = 0b001;
constexpr std::uint8_t MODE_SYMMETRIC_PASSIVE = 0b010;
constexpr std::uint8_t MODE_CLIENT = 0b011;
constexpr std::uint8_t MODE_SERVER = 0b100;
constexpr std::uint8_t MODE_BROADCAST = 0b101;
constexpr std::uint8_t MODE_RESERVED_CONTROL = 0b110;
constexpr std::uint8_t MODE_RESERVED_PRIVATE = 0b111;

//
// Performs the LeapIndicator, VersionNumber and Mode bit shifting for the attributes byte.
//
#define NTP_CALCULATE_ATTRIBUTES(LeapIndicator, VersionNumber, Mode) \
	((LeapIndicator << 6) | (VersionNumber << 3) | Mode)

//
// Reverses the bit shifting for the passed attributes byte and writes the 3 values to the passed arguments.
//
#define NTP_REVERSE_ATTRIBUTES(Attributes, LeapIndicator, VersionNumber, Mode) \
    do \
	{ \
        (LeapIndicator) = (Attributes >> 6) & 0x03; \
        (VersionNumber) = (Attributes >> 3) & 0x07; \
        (Mode) = (Attributes & 0x07); \
    } \
	while (0)

//
// Stratum.
//
constexpr std::uint8_t STRATUM_UNSPECIFIED = 0;
constexpr std::uint8_t STRATUM_PRIMARY_REF = 1;
constexpr std::uint8_t STRATUM_SECONDARY_REF = 2;

//
// PollIntervall.
// "This is a signed integer indicating the minimum interval between transmitted messages, in seconds as a power of two."
//
constexpr std::int8_t NTP_MINPOLL = -6;
constexpr std::int8_t NTP_MAXPOLL = 17;
constexpr std::int8_t POLL_INTERVAL_1SEC = 0;
constexpr std::int8_t POLL_INTERVAL_2SEC = 1;
constexpr std::int8_t POLL_INTERVAL_4SEC = 2;
constexpr std::int8_t POLL_INTERVAL_8SEC = 3;

#define NTP_CALCULATE_POLL_INTERVAL(Microseconds) \
    ((Microseconds) < 0 ? NTP_MINPOLL : \
    (std::log2(static_cast<double>(Microseconds) / 1000) < NTP_MINPOLL ? NTP_MINPOLL : \
    (std::log2(static_cast<double>(Microseconds) / 1000) > NTP_MAXPOLL ? NTP_MAXPOLL : \
    std::log2(static_cast<double>(Microseconds) / 1000))))

//
// Precision.
// "This is a signed integer indicating the precision of the various clocks, in seconds to the nearest power of two.
// The value must be rounded to the next larger power of two;
// for instance, a 50 - Hz(20 ms) or 60 - Hz(16.67ms) power - frequency clock would be assigned the value - 5 (31.25 ms),
// while a 1000 - Hz(1 ms) crystal - controlled clock would be assigned the value - 9 (1.95 ms)."
//
#define HZ_TO_SEC(Hz) \
	((1000.0 / (Hz)) / 1000)
#define EXPONENT_OF_NEXT_POWER_OF_2(x) \
	(static_cast<int>(std::ceil(std::log2(x))))

#define NTP_CALCULATE_PRECISION(Hz) \
	EXPONENT_OF_NEXT_POWER_OF_2(HZ_TO_SEC(Hz))

//
// NTP timestamp.
// Uses big-endian format.
//
#pragma pack(push, 1)
typedef union _NTP_TIMESTAMP
{
	struct
	{
		std::uint32_t High;
		std::uint32_t Low;
	};
	std::uint64_t Full;
} NTP_TIMESTAMP, * PNTP_TIMESTAMP;
#pragma pack(pop)

//
// The header struct uses big-endian format.
// OriginateTimestamp is a random value, due to security concerns. Reference: https://www.ietf.org/archive/id/draft-ietf-ntp-data-minimization-04.txt.
//
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
	NTP_TIMESTAMP ReferenceTimestamp;				// 64 bits
	NTP_TIMESTAMP OriginateTimestamp;				// 64 bits
	NTP_TIMESTAMP ReceiveTimestamp;					// 64 bits
	NTP_TIMESTAMP TransmitTimestamp;				// 64 bits
} NTP_3_HEADER, * PNTP_3_HEADER;
#pragma pack(pop)

namespace NTPv3
{
	//
	// Generates an NTP timestamp (big-endian format.
	//
	NTP_TIMESTAMP
	GenerateTimestamp(
		void
	);
}