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
// Polling intervals for the server querying its root clock.
//
constexpr std::uint64_t NTP_SERVER_ROOT_POLL_MIN = 64;
constexpr std::uint64_t NTP_SERVER_ROOT_POLL_MAX = 1024;

//
// All following data taken from https://datatracker.ietf.org/doc/html/rfc1305#appendix-A.
// To-Do: Add values for other members of the struct.

// 
// Attributes LeadIndicator.
// LI_NO_WARNING: No leap second.
// LI_LAST_MINUTE_61: positive leap second, when leap second is added to UTC.
// LI_LAST_MINUTE_59: negative leap second, when leap second is removed from UTC (has never occurred yet).
// LI_ALARM_CONDITION should never occur.
//
constexpr std::uint8_t LI_NO_WARNING = 0b00;
constexpr std::uint8_t LI_LAST_MINUTE_61 = 0b01;
constexpr std::uint8_t LI_LAST_MINUTE_59 = 0b10;
constexpr std::uint8_t LI_ALARM_CONDITION = 0b11;

//
// Attributes VersionNumber.
//
constexpr std::uint8_t VN_3 = 0b011;
constexpr std::uint8_t VN_4 = 0b100;

//
// Attributes Mode.
//
constexpr std::uint8_t MODE_RESERVED = 0b000;							// Server doesn't accept this
constexpr std::uint8_t MODE_SYMMETRIC_ACTIVE = 0b001;
constexpr std::uint8_t MODE_SYMMETRIC_PASSIVE = 0b010;					// Server doesn't accept this
constexpr std::uint8_t MODE_CLIENT = 0b011;
constexpr std::uint8_t MODE_SERVER = 0b100;								// Server doesn't accept this
constexpr std::uint8_t MODE_BROADCAST = 0b101;							// Server doesn't accept this
constexpr std::uint8_t MODE_RESERVED_CONTROL = 0b110;					// Server doesn't accept this
constexpr std::uint8_t MODE_RESERVED_PRIVATE = 0b111;					// Server doesn't accept this

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
constexpr std::int8_t NTP_MINPOLL = 6;
constexpr std::int8_t NTP_MAXPOLL = 10;
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
// RootDelay.
// Sum of the:
// - time for the request to travel to the secondary server.
// - time for the secondary server to process the request.
// - time for the response to travel back from the secondary server.
// We can just define a macro that measures the time it takes to execute code passed.
//
#define NTP_CALCULATE_ROOT_DELAY(Code) \
	({ \
		auto Start = std::chrono::high_resolution_clock::now(); \
		Code \
		auto End = std::chrono::high_resolution_clock::now(); \
		std::chrono::duration<double> Elapsed = End - Start; \
		Elapsed.count(); \
	})

#define DOUBLE_TO_FIXEDPOINT(Double) \
	static_cast<std::int32_t>(std::round((Double) * (1 << 16)))

#define FIXEDPOINT_TO_DOUBLE(FixedPoint) \
	((static_cast<double>(FixedPoint)) / (1 << 16))

//
// Precision.
// "This is a signed integer indicating the precision of the various clocks, in seconds to the nearest power of two.
// The value must be rounded to the next larger power of two;
// for instance, a 50 - Hz(20 ms) or 60 - Hz(16.67ms) power - frequency clock would be assigned the value - 5 (31.25 ms),
// while a 1000 - Hz(1 ms) crystal - controlled clock would be assigned the value - 9 (1.95 ms)."
//
#define HZ_TO_SEC(Hz) \
	((1000.0 / (Hz)) / 1000)

#define EXPONENT_OF_NEXT_POWER_OF_2(X) \
	(static_cast<int>(std::ceil(std::log2(X))))

#define NTP_CALCULATE_PRECISION(Hz) \
	EXPONENT_OF_NEXT_POWER_OF_2(HZ_TO_SEC(Hz))

#define SEC_TO_HZ(Sec) \
	(1000.0 / ((Sec) * 1000))

#define APPROX_POWER_OF_2(X) \
	(std::pow(2.0, static_cast<double>(X)))

#define NTP_REVERSE_PRECISION(Precision) \
	SEC_TO_HZ(APPROX_POWER_OF_2(Precision))

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
// RootDelay.
// RootDelay is calculated by the server by comparing its primary reference clock against its system time (e.g., std::time).
// The custom server implementation doesn't have a hardware based reference clock, therefore it will set it to a constant value.
// The constant value will be 0, copying the behavior of time.google.com.
//
constexpr std::int32_t NTP_ROOT_DELAY = 0;

//
// RootDispersion.
// "This is a 32-bit signed fixed-point number indicating the maximum error relative to the primary reference source,
// in seconds with fraction point between bits 15 and 16"
// 

//
// The header struct uses big-endian format. 
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
	NTP_TIMESTAMP
	GenerateTimestamp(
		void
	);

	NTP_TIMESTAMP
	GenerateRandomTimestamp(
		void
	);

	std::uint32_t
	FractionToMs(
		std::uint32_t Fraction
	);

	bool
	IsLeapSecond(
		NTP_TIMESTAMP Timestamp
	);
}

typedef struct _LEAP_SECOND
{
	std::uint32_t Seconds;
	std::int32_t Dtai;
} LEAP_SECOND, * PLEAD_SECOND;