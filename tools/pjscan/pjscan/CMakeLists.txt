# Copyright 2012 Nedim Srndic, University of Tuebingen
# 
# This file is part of pjscan.
#
# pjscan is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
# 
# pjscan is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with pjscan.  If not, see <http://www.gnu.org/licenses/>.

cmake_minimum_required(VERSION 2.8)
project(pjscan)
set(PJSCAN_VERSION "1.0")

# A macro for aborting if a required library is not found
macro(require_library RL_LIBRARY_NAME)
	find_library(RL_LIB ${RL_LIBRARY_NAME})
	if (RL_LIB STREQUAL "RL_LIB-NOTFOUND")
		message(FATAL_ERROR "The library '${RL_LIBRARY_NAME}' is required, not found")
	else (RL_LIB STREQUAL "RL_LIB-NOTFOUND")
		message(STATUS "Found library '${RL_LIBRARY_NAME}'")
	endif (RL_LIB STREQUAL "RL_LIB-NOTFOUND")
	unset(RL_LIB CACHE)
	unset(RL_LIB)
endmacro(require_library)

# Some libraries are required
require_library(stem)
require_library(svm_oc)
require_library(pdfjs)
require_library(boost_thread)
require_library(boost_filesystem)
require_library(boost_system)
require_library(argtable2)

# Set some important names
set(PJSCAN_EXECUTABLE_NAME pjscan)
set(PJSCAN_CHILD_EXECUTABLE_NAME pjscan-child)
set(PJSCAN_REQUIRED_LIBRARIES stem boost_thread boost_filesystem boost_system svm_oc pdfjs argtable2)
set(PJSCAN_CHILD_REQUIRED_LIBRARIES pdfjs)

# Set default compile flags for GCC
if(CMAKE_COMPILER_IS_GNUCXX)
    message(STATUS "GCC detected, adding compile flags")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pedantic -Wformat=2 -Wall -Wextra")
endif(CMAKE_COMPILER_IS_GNUCXX)

# Source directory
add_subdirectory(src)
