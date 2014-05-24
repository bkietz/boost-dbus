# Copyright (c) Benjamin Kietzman (github.com/bkietz)
#
# Distributed under the Boost Software License, Version 1.0. (See accompanying
# file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

project(boost-asio-dnssd CXX)

###############
# CMake options
cmake_minimum_required(VERSION 2.8)

###############
# C++ options
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++0x")
include_directories(${CMAKE_SOURCE_DIR}/include)

###############
# import Boost
find_package(Boost 1.54 REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})
link_directories(${Boost_LIBRARY_DIRS})

###############
# import D-Bus
find_package(PkgConfig)
pkg_check_modules(DBus dbus-1)
include_directories(${DBus_INCLUDE_DIRS})
link_directories(${DBus_LIBRARY_DIRS})

##############
# Trivial executable
add_executable(test "test.cpp")
target_link_libraries(test -lboost_system ${DBus_LIBRARIES})

