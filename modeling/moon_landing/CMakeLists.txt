cmake_minimum_required(VERSION 3.16.0)

project(MoonLanding LANGUAGES CXX)

option(MOON_FETCHING "Enable downloading third party libraries" ON)
option(MOON_LOGGING "Enable logging" ON)
option(MOON_TRACE_LOGGING "Enable detailed logging. Causes performance issues" OFF)
option(MOON_PCH "Enable building using a precompiled header to speed up compilation time" ON)

set(MOON_DIR ${CMAKE_CURRENT_LIST_DIR})

list(APPEND CMAKE_MODULE_PATH "${MOON_DIR}/cmake")

add_subdirectory(lib)
add_subdirectory(gui)
