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
#include <chrono>
#include <cstring>

#include "NTPv3.hpp"
#include "Util.hpp"

namespace Client
{
	std::string
	GetIp(
		const std::string& Domain
	);
}