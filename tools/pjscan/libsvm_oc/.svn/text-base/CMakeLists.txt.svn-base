# Copyright 2012 Nedim Srndic, Pavel Laskov, University of Tuebingen
# 
# This file is part of libsvm_oc.
#
# libsvm_oc is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
# 
# libsvm_oc is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with libsvm_oc.  If not, see <http://www.gnu.org/licenses/>.

cmake_minimum_required(VERSION 2.8)
project(libsvm_oc)
set(SVM_OC_VERSION "1.0")

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

# If stem is not installed, do not proceed
require_library(stem)

# Set some important names
set(SVM_OC_SHARED_LIBRARY_NAME svm_oc)
set(SVM_OC_STATIC_LIBRARY_NAME svm_oc-static)
set(SVM_OC_REQUIRED_LIBRARIES stem)

# Set default compile flags for GCC
if(CMAKE_COMPILER_IS_GNUCXX)
    message(STATUS "GCC detected, adding compile flags")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pedantic -Wformat=2 -Wall -Wextra")
endif(CMAKE_COMPILER_IS_GNUCXX)

# Source directory
add_subdirectory(src)

# Python API directory
if (MAKE_API)
    message(STATUS "Will make Python API")
    add_subdirectory(api)
endif (MAKE_API)
unset(MAKE_API CACHE)
unset(MAKE_API)

# Install include files
install(DIRECTORY src/
        DESTINATION include/svm_oc
        FILES_MATCHING PATTERN "*.h"
                       PATTERN ".svn" EXCLUDE
       )
