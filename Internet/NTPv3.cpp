#include "NTPv3.hpp"

NTP_TIMESTAMP
NTPv3::GenerateTimestamp(
	void
)
{
	std::time_t CurrentTime = std::time(nullptr);
	NTP_TIMESTAMP Timestamp = { 0 };
	Timestamp.High = Util::SwitchEndianness32(static_cast<uint32_t>(CurrentTime + NTP_TIMESTAMP_DELTA));
	Timestamp.Low = Util::SwitchEndianness32(static_cast<std::uint32_t>(std::fmod(CurrentTime, 1.0) * (1 << 32)));
	return Timestamp;
}