#include "Client.hpp"

float Client::GetFrequency(void)
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

std::string Client::GetIp(const std::string& Domain)
{
	hostent* Host = gethostbyname(Domain.c_str());

	if (!Host)
	{
		return nullptr;
	}

	in_addr* Address = reinterpret_cast<in_addr*>(Host->h_addr_list[0]);

	if (!Address)
	{
		return nullptr;
	}

	return std::string(inet_ntoa(*Address));
}