#include "Client.hpp"

std::string
Client::GetIp(
	const std::string& Domain
)
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