﻿# CMakeList.txt : CMake project for Internet, include source and define
# project specific logic here.
#

# Add source to this project's executable.
add_executable (Internet   "Entry.cpp" "NTPv3.cpp" "Util.cpp" "Client.cpp")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET Internet PROPERTY CXX_STANDARD 20)
endif()