#include "Internet.hpp"

int main(void)
{
	PNTP_3_HEADER NtpPacket = reinterpret_cast<PNTP_3_HEADER>(calloc(1, sizeof(NTP_3_HEADER)));

	return 0;
}