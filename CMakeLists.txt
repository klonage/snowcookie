cmake_minimum_required(VERSION 2.8.12)
project(snowcookie)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -pedantic -std=c++11")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wl,--no-as-needed")

find_package (Threads)
# enable if needed INCLUDE(FindPkgConfig)


add_subdirectory(edison-board)
