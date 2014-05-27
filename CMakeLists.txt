# Copyright (c) Benjamin Kietzman (github.com/bkietz)
#
# Distributed under the Boost Software License, Version 1.0. (See accompanying
# file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

project(boost-dbus CXX)

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
find_package(PkgConfig REQUIRED)
pkg_check_modules(DBus dbus-1)
include_directories(${DBus_INCLUDE_DIRS})
link_directories(${DBus_LIBRARY_DIRS})

##############
# import GTest
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

##############
# Trivial executable
add_executable(test "test.cpp")
target_link_libraries(test -lboost_system ${DBus_LIBRARIES})

##############
# Tests
function(cxx_test test_name)
  add_executable(${test_name} "test/${test_name}.cpp")
  target_link_libraries(${test_name} -lboost_system ${DBus_LIBRARIES})
  target_link_libraries(${test_name} ${GTEST_BOTH_LIBRARIES})
  target_link_libraries(${test_name} -pthread)
  add_test(${test_name} ${test_name} "--gtest_output=xml:${test_name}.xml")
endfunction(cxx_test)

cxx_test(avahi)
