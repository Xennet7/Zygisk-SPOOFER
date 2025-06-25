cmake_minimum_required(VERSION 3.6)
project(libssaidspoof)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fvisibility=hidden -Wall")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fvisibility=hidden -Wall")

add_definitions(
  -D__ANDROID_API__=29
  -D_LIBCPP_ENABLE_CXX17_REMOVED_FEATURES
)

file(GLOB XHOOK_SRC xhook/*.c)
add_library(libssaidspoof SHARED module.cpp ${XHOOK_SRC})
target_include_directories(libssaidspoof PRIVATE ${CMAKE_SOURCE_DIR}/xhook ${CMAKE_SOURCE_DIR}/zygisk)
target_link_libraries(libssaidspoof log android dl)
