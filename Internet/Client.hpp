#pragma once

#include <iostream>
#include <cstdlib>
#include <ctime>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstdint>
#include <netdb.h>
#include <algorithm>
#include <cmath>
#include <fstream>

#include "NTPv3.hpp"
#include "Util.hpp"

namespace Client
{
	float
	GetFrequency(
		void
	);

	const std::string
	GetIp(
		const std::string& Domain
	);
}