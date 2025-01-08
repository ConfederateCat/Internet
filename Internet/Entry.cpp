#include "Internet.hpp"


int
main(
	void
)
{
	//
	// Create and zero out the NTP packet.
	//
	std::uint8_t NtpPacket[NTP_PACKET_SIZE] = { 0 };
	NtpPacket[0] = NTP_HEADER_BYTE;

	//
	// Configure the server address.
	//
	sockaddr_in ServerAddress{};
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

	ssize_t Sent = sendto(SockFd, NtpPacket, NTP_PACKET_SIZE, 0, reinterpret_cast<sockaddr*>(&ServerAddress), sizeof(ServerAddress));

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
	ssize_t Received = recvfrom(SockFd, NtpPacket, NTP_PACKET_SIZE, 0, reinterpret_cast<sockaddr*>(&ReceiveAddress), &AddressLength);

	if (Received < 0)
	{
		std::cerr << "recvfrom failed" << std::endl;
		close(SockFd);
		return 1;
	}

	//
	// Extract timestamps from the received packet.
	//
	std::uint32_t TransmitTimeSeconds = (NtpPacket[40] << 24) | (NtpPacket[41] << 16) | (NtpPacket[42] << 8) | NtpPacket[43];

	//
	// Convert NTP time to UNIX time.
	//
	time_t UnixTime = static_cast<time_t>(TransmitTimeSeconds - NTP_TIMESTAMP_DELAT);

	std::cout << "Synchronized time: " << ctime(&UnixTime);

	close(SockFd);
	return 0;
}