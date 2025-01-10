#include "NTPv3.hpp"

NTP_TIMESTAMP
NTPv3::GenerateTimestamp(
	void
)
{
	std::time_t CurrentTime = std::time(nullptr);
	NTP_TIMESTAMP Timestamp = { 0 };
	Timestamp.High = Util::SwitchEndianness32(static_cast<uint32_t>(CurrentTime + NTP_TIMESTAMP_DELTA));
	Timestamp.Low = Util::SwitchEndianness32(static_cast<std::uint32_t>(std::fmod(static_cast<double>(CurrentTime), 1.0) * (1 << 32)));
	return Timestamp;
}

NTP_TIMESTAMP
NTPv3::GenerateRandomTimestamp(
	void
)
{
    auto Now = std::chrono::high_resolution_clock::now();                                               // Get the current time in high resolution
    auto Seconds = std::chrono::time_point_cast<std::chrono::seconds>(Now);                             // Retrieve the seconds from the current time
    auto Fraction = Now - Seconds;                                                                      // Calculate the fractional part
    std::time_t CurrentTime = std::chrono::system_clock::to_time_t(Seconds);                            // Convert the seconds to std::time_t
    NTP_TIMESTAMP Timestamp = { 0 };
    Timestamp.High = Util::SwitchEndianness32(static_cast<uint32_t>(CurrentTime + NTP_TIMESTAMP_DELTA));
    Timestamp.Low = Util::SwitchEndianness32(static_cast<uint32_t>(std::chrono::duration_cast<std::chrono::duration<double>>(Fraction).count() * (1 << 32)));

    return Timestamp;
}