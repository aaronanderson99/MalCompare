# Copyright 2012 Nedim Srndic, Pavel Laskov, University of Tuebingen
# 
# This file is part of libstem.
#
# libstem is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
# 
# libstem is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with libstem.  If not, see <http://www.gnu.org/licenses/>.

# Build and run the stem library tests

# If boost_unit_test_framework is not installed, do not proceed
find_library(BOOST_UNIT_TEST_FRAMEWORK_LIB boost_unit_test_framework)

if (BOOST_UNIT_TEST_FRAMEWORK_LIB STREQUAL "BOOST_UNIT_TEST_FRAMEWORK_LIB-NOTFOUND")
    message(FATAL_ERROR "Boost unit test framework library not found - not building tests")
endif (BOOST_UNIT_TEST_FRAMEWORK_LIB STREQUAL "BOOST_UNIT_TEST_FRAMEWORK_LIB-NOTFOUND")

# Unit test header file must also be present
find_file(BOOST_TEST_UNIT_TEST_HPP "boost/test/unit_test.hpp")
if (NOT BOOST_TEST_UNIT_TEST_HPP)
    MESSAGE(FATAL_ERROR "Missing header file boost/test/unit_test.hpp")
endif (NOT BOOST_TEST_UNIT_TEST_HPP)

# Link to boost_unit_test_framework
set(TEST_REQUIRED_LIBRARIES boost_unit_test_framework ${STEM_SHARED_LIBRARY_NAME})

#################### Test 1: NGram ##############################
# Set options
set(NGRAM_TEST_SOURCES NGramTest.cpp)
set(NGRAM_TEST_NAME test_ngram)

# Create executables
add_executable(${NGRAM_TEST_NAME} ${NGRAM_TEST_SOURCES})
target_link_libraries(${NGRAM_TEST_NAME} ${TEST_REQUIRED_LIBRARIES})

#################### Test 2: Kernel ##############################
# Set options
set(KERNEL_TEST_SOURCES KernelTest.cpp)
set(KERNEL_TEST_NAME test_kernel)

# Create executables
add_executable(${KERNEL_TEST_NAME} ${KERNEL_TEST_SOURCES})
target_link_libraries(${KERNEL_TEST_NAME} ${TEST_REQUIRED_LIBRARIES})
    


unset(BOOST_UNIT_TEST_FRAMEWORK_LIB CACHE)
unset(BOOST_UNIT_TEST_FRAMEWORK_LIB)
