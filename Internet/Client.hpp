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
#include <thread>

#include "NTPv3.hpp"
#include "Util.hpp"

extern bool gDebug;
extern std::int32_t gPort;
extern bool gIp;
extern bool gUrl;
extern char gAddress[64];