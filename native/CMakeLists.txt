
cmake_minimum_required(VERSION 3.6)
project(ssaidspoof)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(ssaidspoof SHARED ssaid_imei_mac_spoofer.cpp)

find_library(log-lib log)

target_link_libraries(ssaidspoof ${log-lib})
