#include "Util.hpp"

std::uint32_t
Util::SwitchEndianness32(
    std::uint32_t Value
)
{
    return ((Value >> 24) & 0x000000FF) |               // Move byte 0 to byte 3
        ((Value >> 8) & 0x0000FF00) |                   // Move byte 1 to byte 2
        ((Value << 8) & 0x00FF0000) |                   // Move byte 2 to byte 1
        ((Value << 24) & 0xFF000000);                   // Move byte 3 to byte 0
}

std::uint16_t
Util::SwitchEndianness16(
    std::uint16_t Value
)
{
    return (Value >> 8) | (Value << 8);                 // Swap the high and low bytes
}

std::uint32_t
Util::SwitchEndianness64(
    std::uint64_t Value
)
{
    return ((Value >> 56) & 0x00000000000000FF) |
        ((Value >> 40) & 0x000000000000FF00) |
        ((Value >> 24) & 0x0000000000FF0000) |
        ((Value >> 8) & 0x00000000FF000000) |
        ((Value << 8) & 0x000000FF00000000) |
        ((Value << 24) & 0x0000FF0000000000) |
        ((Value << 40) & 0x00FF000000000000) |
        ((Value << 56) & 0xFF00000000000000);
}