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