
cmake_minimum_required(VERSION 3.6)
project(libssaidspoof)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fvisibility=hidden -Wall")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fvisibility=hidden -Wall")
file(GLOB XHOOK_SRC xhook/*.c)
add_library(libssaidspoof SHARED module.cpp ${XHOOK_SRC})
target_include_directories(libssaidspoof PRIVATE ${CMAKE_SOURCE_DIR}/xhook)
target_link_libraries(libssaidspoof log android dl)
