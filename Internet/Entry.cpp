#include "Client.hpp"

bool gDebug = false;
std::int32_t gPort = NTP_PORT;
bool gIp = false;
bool gUrl = false;
char gAddress[64] = "127.0.0.1";

bool
HandleArgs(
	int ArgC,
	char* ArgV[]
)
{
	if (ArgC == 1)
	{
		return true;
	}

	for (int i = 1; i < ArgC; i++)
	{
		//
		// -debug
		//
		if (std::strcmp(ArgV[i], "-debug") == 0)
		{
			gDebug = true;
		}
		//
		// -port=<port-nr>
		//
		else if (std::strncmp(ArgV[i], "-port=", 6) == 0)
		{
			gPort = std::atoi(ArgV[i] + 6);
		}
		//
		// -type=<url> or -type<ip>
		//
		else if (std::strncmp(ArgV[i], "-type=", 6) == 0)
		{
			const char* Type = ArgV[i] + 6;

			if (std::strncmp(Type, "url", 3) == 0)
			{
				gUrl = true;
				gIp = false;
			}
			else if (std::strncmp(Type, "ip", 2) == 0)
			{
				gIp = true;
				gUrl = false;
			}
		}
		//
		// -address<address>
		//
		else if (std::strncmp(ArgV[i], "-address=", 9) == 0)
		{
			const char* Address = ArgV[i] + 9;

			if (std::strlen(Address) < sizeof(gAddress))
			{
				std::strncpy(gAddress, Address, sizeof(gAddress) - 1);
				gAddress[sizeof(gAddress) - 1] = '\0';
			}
			else
			{
				std::cout << "Invalid address length (max. " << sizeof(gAddress) - 1 << " characters)" << std::endl;
				return false;
			}
		}
		else if (std::strcmp(ArgV[i], "-help") == 0)
		{
			std::cout << "Usage: ./Server [-port=<port-nr>] [-type=<type>] [-address<addr>] [-debug]" << std::endl
				<< "-port=<port-nr> Specifies the port number to host the server on." << std::endl
				<< "-type=<type>	Specifies the type (either 'url' or 'ip')." << std::endl
				<< "-address=<addr>	Specifies the address." << std::endl
				<< "-debug          Enables detailed log messages in the console (recommended)." << std::endl;
			return false;
		}
		else
		{
			std::cout << "Invalid argument: " << ArgV[i] << std::endl;
			return false;
		}
	}

	return true;
}

int
main(
	int ArgC,
	char* ArgV[]
)
{
	std::cout << "NTP v3 Client Implementation (RFC 1305)" << std::endl;

	//
	// Handle the command line arguments.
	//
	if (!HandleArgs(ArgC, ArgV))
	{
		return 1;
	}

	if (gDebug)
	{
		std::cout << "[+] Client loaded" << std::endl;
	}

	if (gUrl && std::strcmp(gAddress, "127.0.0.1") == 0) // If type was set to URL but the address wasn't set
	{
		gUrl = false;
		
		if (gDebug)
		{
			std::cout << "[!] URL not specified, defaulting to localhost" << std::endl;
		}
	}

	//
	// Create and zero out the NTP packet.
	// Set the leap indicator to LI_ALARM_CONDITION. The server will determine its own leap indicator and send it back.
	//
	NTP_3_HEADER Packet = { 0 };
	Packet.Attributes = NTP_CALCULATE_ATTRIBUTES(LI_ALARM_CONDITION, VN_3, MODE_CLIENT);

	//
	// Configure the server address.
	// For debugging purpose, we can use local IP (127.0.0.1) and port 12345.
	//
	sockaddr_in ServerAddress = { 0 };
	ServerAddress.sin_family = AF_INET;
	ServerAddress.sin_port = Util::SwitchEndianness16(gPort);

	if (gUrl)
	{
		std::strncpy(gAddress, Util::GetIp(gAddress), sizeof(gAddress) - 1);
		gAddress[sizeof(gAddress) - 1] = '\0';
	}

	if (inet_pton(AF_INET, gAddress, &ServerAddress.sin_addr) <= 0)
	{
		std::cerr << "[-] Failed to convert address '" << gAddress << "' to a valid network address" << std::endl;
		return 1;
	}

	if (gDebug)
	{
		std::cout << "[+] Sever address configured" << std::endl;
	}

	//
	// Create the UDP socket.
	//
	std::int32_t Socket = socket(AF_INET, SOCK_DGRAM, 0);

	if (Socket < 0)
	{
		std::cerr << "[-] Failed to create a UDP socket" << std::endl;
		return 1;
	}
	
	//
	// Set the poll interval.
	// Essentially a hint from the client how frequently it's willing to query the server for time.
	//
	Packet.PollInterval = NTP_CALCULATE_POLL_INTERVAL(64000);

	if (gDebug)
	{
		std::cout << "[+] Poll interval set" << std::endl;
	}

	//
	// Set the precision.
	// The server will set its own precision in the response.
	//
	Packet.Precision = NTP_CALCULATE_PRECISION(Util::GetFrequency());

	if (gDebug)
	{
		std::cout << "[+] Frequency set" << std::endl;
	}

	//
	// Generate a random transmit timestamp and save it.
	// The server will copy that timestamp to the originate timestamp.
	// The client shall verify the originate timestamp of the returned packet with this timestamp.
	// Source: https://www.ietf.org/archive/id/draft-ietf-ntp-data-minimization-04.txt
	//
	NTP_TIMESTAMP RandomTimestamp = NTPv3::GenerateRandomTimestamp();
	Packet.TransmitTimestamp.Full = RandomTimestamp.Full;

	if (gDebug)
	{
		std::cout << "[+] Transmit timestamp set" << std::endl;
	}

	//
	// Send the packet.
	//
	ssize_t Sent = sendto(Socket, &Packet, sizeof(NTP_3_HEADER), 0, reinterpret_cast<sockaddr*>(&ServerAddress), sizeof(ServerAddress));

	if (Sent < 0)
	{
		std::cerr << "[-] Failed to send the packet" << std::endl;
		close(Socket);
		return 1;
	}

	if (gDebug)
	{
		std::cout << "[+] Packet sent:" << std::endl;
		Util::PrintBytes(&Packet, sizeof(NTP_3_HEADER), true);
	}

	//
	// Receive the NTP response packet.
	//
	sockaddr_in ReceiveAddress{};
	socklen_t AddressLength = sizeof(ReceiveAddress);
	ssize_t Received = recvfrom(Socket, &Packet, sizeof(NTP_3_HEADER), 0, reinterpret_cast<sockaddr*>(&ReceiveAddress), &AddressLength);

	if (Received < 0)
	{
		std::cerr << "[-] Failed to receive a response from the NTP server" << std::endl;
		close(Socket);
		return 1;
	}

	if (gDebug)
	{
		std::cout << "[+] Packet received:" << std::endl;
		Util::PrintBytes(&Packet, sizeof(NTP_3_HEADER), true);
	}

	//
	// Verify the originate timestamp with the first transmit timestamp generated by the client.
	//
	if (RandomTimestamp.Full != Packet.OriginateTimestamp.Full)
	{
		std::cerr << "[-] Failed to verify the NTP response. Originate timestamp mismatch" << std::endl;
		close(Socket);
		return 1;
	}

	std::time_t TransmitTimestamp = Util::SwitchEndianness32(Packet.TransmitTimestamp.High) - NTP_TIMESTAMP_DELTA;
	std::tm* UtcTime = std::gmtime(&TransmitTimestamp);

	if (UtcTime)
	{
		std::cout << "Transmit timestamp: (UTC): " << std::asctime(UtcTime);
	}
	else
	{
		std::cerr << "[Error] Failed to convert the transmit timestamp to UTC format" << std::endl;
	}

	close(Socket);
	return 0;
}