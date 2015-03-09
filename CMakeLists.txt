cmake_minimum_required(VERSION 2.8.12)
project(snowcookie)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -pedantic -std=c++11")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wl,--no-as-needed")

find_package (Threads)
# enable if needed INCLUDE(FindPkgConfig)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
find_package(Qt5Widgets REQUIRED)
ADD_DEFINITIONS(-DQT_NO_KEYWORDS)

include_directories(${CMAKE_SOURCE_DIR})

add_subdirectory(edison-board)
add_subdirectory(diagnostic)
