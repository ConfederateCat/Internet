#pragma once

#include "Internet.hpp"

constexpr std::int32_t NTP_PORT = 123;
constexpr std::uint64_t NTP_PACKET_SIZE = 48;
constexpr std::uint32_t NTP_TIMESTAMP_DELAT = 2208988800;

constexpr std::uint8_t LI_NO_WARNING = 0b00;
constexpr std::uint8_t VN_3 = 0b011;
constexpr std::uint8_t MODE_CLIENT = 0b011;

constexpr std::uint8_t NTP_HEADER_BYTE = (LI_NO_WARNING << 6) | (VN_3 << 3) | MODE_CLIENT;