cmake_minimum_required(VERSION 2.8.12)
project(snowcookie)

option(DIAG "build diagnostic tool" OFF)
option(WITH_WIFI_AUTODISABLING "enable wifi autodisable feature" ON)

if(WITH_WIFI_AUTODISABLING)
	add_definitions(-DWIFI_AUTODISABLING)
endif()

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -pedantic -std=c++11")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wl,--no-as-needed")

find_package (Threads)
# enable if needed INCLUDE(FindPkgConfig)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

if (BUILD_DIAG)
	set(CMAKE_AUTOMOC ON)
	find_package(Qt5Widgets REQUIRED)
	ADD_DEFINITIONS(-DQT_NO_KEYWORDS)
endif (BUILD_DIAG)

include_directories(${CMAKE_SOURCE_DIR})

add_subdirectory(protocol)
add_subdirectory(edison-board)

if (BUILD_DIAG)
	add_subdirectory(diagnostic)
endif (BUILD_DIAG)
