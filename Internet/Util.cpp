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

float
Util::GetFrequency(
    void
)
{
    std::ifstream CpuInfo("/proc/cpuinfo");
    std::string Key = "cpu MHz";
    std::string Line;

    while (std::getline(CpuInfo, Line))
    {
        if (Line.find(Key) != std::string::npos)
        {
            return std::stoi(Line.substr(Line.find(":") + 1));
        }
    }

    return 0;
}

char*
Util::GetIp(
    const char* Domain
)
{
    hostent* Host = gethostbyname(Domain);

    if (!Host)
    {
        return nullptr;
    }

    in_addr* Address = reinterpret_cast<in_addr*>(Host->h_addr_list[0]);

    if (!Address)
    {
        return nullptr;
    }

    return inet_ntoa(*Address);
}

void
Util::PrintBytes(
    void* Address,
    std::size_t Length,
    bool PrintLine
)
{
    std::uint8_t* Ptr = static_cast<uint8_t*>(Address);
    std::cout << std::uppercase;

    for (std::size_t i = 0; i < Length; i += 16)
    {
        if (PrintLine)
        {
            std::cout << "0x" << std::setw(4) << std::setfill('0') << std::hex << i << ": ";
        }

        std::size_t BytesToPrint = std::min<std::size_t>(16, Length - i);

        for (std::size_t j = 0; j < BytesToPrint; ++j)
        {
            std::cout << std::setw(2) << std::setfill('0') << std::hex << (int)Ptr[i + j] << " ";
        }

        if (BytesToPrint < 16)
        {
            for (std::size_t j = 0; j < 16 - BytesToPrint; j++)
            {
                std::cout << "   ";
            }
        }

        std::cout << " ";

        for (std::size_t j = 0; j < BytesToPrint; ++j)
        {
            char c = Ptr[i + j];
            std::cout << (std::isprint(c) ? c : '.');
        }

        std::cout << std::endl;
    }
}