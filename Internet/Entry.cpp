#include "Internet.hpp"



int
main(
	void
)
{
	//
	// Create and zero out the NTP packet.
	//
	NTP_3_HEADER Packet = { 0 };
	Packet.Attributes = NTP_CALCULATE_ATTRIBUTES(LI_NO_WARNING, VN_3, MODE_CLIENT);

	//
	// Configure the server address.
	//
	sockaddr_in ServerAddress = { 0 };
	ServerAddress.sin_family = AF_INET;
	ServerAddress.sin_port = htons(NTP_PORT);

	if (inet_pton(AF_INET, "216.239.35.0", &ServerAddress.sin_addr) <= 0)
	{
		std::cerr << "inet_pton failed" << std::endl;
		return 1;
	}

	//
	// Create the UDP socket.
	//
	std::int32_t SockFd = socket(AF_INET, SOCK_DGRAM, 0);

	if (SockFd < 0)
	{
		std::cerr << "Socket creation failed" << std::endl;
		return 1;
	}

	//
	// Set OriginateTimestamp.
	// Example: Unix time 1633514001.123456
	// High part will be 1633514001 + NTP_TIMESTAMP_DELTA in big-endian format
	// Low part will be the fractional part 0.123456 multiplied by 2^32, in big-endian format
	//
	std::time_t CurrentTime = std::time(nullptr);
	Packet.OriginateTimestamp.High = Util::SwitchEndianness32(static_cast<uint32_t>(CurrentTime + NTP_TIMESTAMP_DELTA));
	Packet.OriginateTimestamp.Low = Util::SwitchEndianness32(static_cast<std::uint32_t>(std::fmod(CurrentTime, 1.0) * (1 << 32)));

	//
	// Set the poll interval.
	//
	Packet.PollInterval = NTP_CALCULATE_POLL_INTERVAL(1000);

	//
	// Set the precision.
	//
	Packet.Precision = NTP_CALCULATE_PRECISION(1000);

	//
	// Send the packet.
	//
	ssize_t Sent = sendto(SockFd, &Packet, sizeof(NTP_3_HEADER), 0, reinterpret_cast<sockaddr*>(&ServerAddress), sizeof(ServerAddress));

	if (Sent < 0)
	{
		std::cerr << "sendto failed" << std::endl;
		close(SockFd);
		return 1;
	}

	//
	// Receive the NTP response packet.
	//
	sockaddr_in ReceiveAddress{};
	socklen_t AddressLength = sizeof(ReceiveAddress);
	ssize_t Received = recvfrom(SockFd, &Packet, sizeof(NTP_3_HEADER), 0, reinterpret_cast<sockaddr*>(&ReceiveAddress), &AddressLength);

	if (Received < 0)
	{
		std::cerr << "recvfrom failed" << std::endl;
		close(SockFd);
		return 1;
	}

	//
	// Extract timestamps from the received packet.
	// Timestamps are in big endian format, we have to convert them to little endian format first.
	//
	std::time_t TransmitTimestamp = Util::SwitchEndianness32(Packet.TransmitTimestamp.High) - NTP_TIMESTAMP_DELTA;
	std::cout << "TransmitTimestamp: " << std::ctime(&TransmitTimestamp) << std::endl;

	close(SockFd);
	return 0;
}