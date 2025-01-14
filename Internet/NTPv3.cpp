#include "Client.hpp"

/*
https://data.iana.org/time-zones/data/leap-seconds.list
#NTP Time      DTAI    Day Month Year
#
2272060800      10      # 1 Jan 1972
2287785600      11      # 1 Jul 1972
2303683200      12      # 1 Jan 1973
2335219200      13      # 1 Jan 1974
2366755200      14      # 1 Jan 1975
2398291200      15      # 1 Jan 1976
2429913600      16      # 1 Jan 1977
2461449600      17      # 1 Jan 1978
2492985600      18      # 1 Jan 1979
2524521600      19      # 1 Jan 1980
2571782400      20      # 1 Jul 1981
2603318400      21      # 1 Jul 1982
2634854400      22      # 1 Jul 1983
2698012800      23      # 1 Jul 1985
2776982400      24      # 1 Jan 1988
2840140800      25      # 1 Jan 1990
2871676800      26      # 1 Jan 1991
2918937600      27      # 1 Jul 1992
2950473600      28      # 1 Jul 1993
2982009600      29      # 1 Jul 1994
3029443200      30      # 1 Jan 1996
3076704000      31      # 1 Jul 1997
3124137600      32      # 1 Jan 1999
3345062400      33      # 1 Jan 2006
3439756800      34      # 1 Jan 2009
3550089600      35      # 1 Jul 2012
3644697600      36      # 1 Jul 2015
3692217600      37      # 1 Jan 2017
*/
const std::vector<LEAP_SECOND> gLeapSeconds =
{
    { 2272060800, 10 }, { 2287785600, 11 }, { 2303683200, 12 },
    { 2335219200, 13 }, { 2366755200, 14 }, { 2398291200, 15 },
    { 2429913600, 16 }, { 2461449600, 17 }, { 2492985600, 18 },
    { 2524521600, 19 }, { 2571782400, 20 }, { 2603318400, 21 },
    { 2634854400, 22 }, { 2698012800, 23 }, { 2776982400, 24 },
    { 2840140800, 25 }, { 2871676800, 26 }, { 2918937600, 27 },
    { 2950473600, 28 }, { 2982009600, 29 }, { 3029443200, 30 },
    { 3076704000, 31 }, { 3124137600, 32 }, { 3345062400, 33 },
    { 3439756800, 34 }, { 3550089600, 35 }, { 3644697600, 36 },
    { 3692217600, 37 }
};

NTP_TIMESTAMP
NTPv3::GenerateRandomTimestamp(
    void
)
{
    NTP_TIMESTAMP Timestamp = { 0 };
    Timestamp.High = std::rand();
    Timestamp.Low = std::rand();
    return Timestamp;
}

NTP_TIMESTAMP
NTPv3::GenerateTimestamp(
    void
)
{
    auto Now = std::chrono::system_clock::now();                                                                                                                // Get the current system time
    auto Seconds = std::chrono::time_point_cast<std::chrono::seconds>(Now);                                                                                     // Retrieve the seconds from the current time
    auto Fraction = Now - Seconds;                                                                                                                              // Calculate the fractional part
    std::time_t CurrentTime = std::chrono::system_clock::to_time_t(Seconds);                                                                                    // Convert the seconds to std::time_t
    NTP_TIMESTAMP Timestamp = { 0 };
    Timestamp.High = Util::SwitchEndianness32(static_cast<uint32_t>(CurrentTime + NTP_TIMESTAMP_DELTA));                                                        // Adjust for NTP epoch
    Timestamp.Low = Util::SwitchEndianness32(static_cast<uint32_t>(std::chrono::duration_cast<std::chrono::duration<double>>(Fraction).count() * (1LL << 32))); // Fractional part

    return Timestamp;
}

std::uint32_t
NTPv3::FractionToMs(
    std::uint32_t Fraction
)
{
    return (Fraction * 1.0E6) / (1LL << 32);
}

bool
NTPv3::IsLeapSecond(
    NTP_TIMESTAMP Timestamp
)
{
    return std::any_of(gLeapSeconds.begin(), gLeapSeconds.end(), [Timestamp](const LEAP_SECOND& Leap)
        {
            return Leap.Seconds == Util::SwitchEndianness32(Timestamp.High);
        });
}