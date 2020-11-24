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

cmake_minimum_required(VERSION 2.8)
project(libstem)
set(STEM_VERSION "1.0")

# Set some important names
set(STEM_SHARED_LIBRARY_NAME stem)
set(STEM_STATIC_LIBRARY_NAME stem-static)

# Set default compile flags for GCC
if(CMAKE_COMPILER_IS_GNUCXX)
    message(STATUS "GCC detected, adding compile flags")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pedantic -Wformat=2 -Wall -Wextra")
endif(CMAKE_COMPILER_IS_GNUCXX)

# Source directory
add_subdirectory(src)

# Test directory
if (MAKE_TESTS)
    message(STATUS "Will make tests")
    add_subdirectory(test)
endif (MAKE_TESTS)
unset(MAKE_TESTS CACHE)
unset(MAKE_TESTS)

# Python API directory
if (MAKE_API)
    message(STATUS "Will make Python API")
    add_subdirectory(api)
endif (MAKE_API)
unset(MAKE_API CACHE)
unset(MAKE_API)

# Install include files
install(DIRECTORY src/
        DESTINATION include/stem
        FILES_MATCHING PATTERN "*.h"
                       PATTERN ".svn" EXCLUDE
       )
