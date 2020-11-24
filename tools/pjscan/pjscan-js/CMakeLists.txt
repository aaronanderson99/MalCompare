# Copyright 2012 Nedim Srndic, University of Tuebingen
# 
# This file is part of pjscan-js.
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
# along with pjscan-js.  If not, see <http://www.gnu.org/licenses/>.

include(ExternalProject)
cmake_minimum_required(VERSION 2.8)
project(pjscan-js)

message(STATUS "Making pjscan-js...")

# Set default compile flags for GCC
if(CMAKE_COMPILER_IS_GNUCXX)
    message(STATUS "GCC detected, adding compile flags")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pedantic -Wformat=2 -Wall -Wextra")
endif(CMAKE_COMPILER_IS_GNUCXX)

# Download, patch and configure Mozilla SpiderMonkey
set(PJSCAN_JS_PROGRAM "pjscan-js")
set(PJSCAN_JS_DOWNLOAD_URL "http://ftp.mozilla.org/pub/mozilla.org/js/js185-1.0.0.tar.gz")
ExternalProject_Add(
	${PJSCAN_JS_PROGRAM}
	URL ${PJSCAN_JS_DOWNLOAD_URL}
	PATCH_COMMAND patch -p1 -i ${PROJECT_SOURCE_DIR}/pjscan-js.patch
	SOURCE_DIR ${CMAKE_BINARY_DIR}/js-1.8.5
	CONFIGURE_COMMAND <SOURCE_DIR>/js/src/configure --disable-shared-js --disable-tests
	INSTALL_COMMAND ""
)

# Install pjscan-js
ExternalProject_Get_Property(${PJSCAN_JS_PROGRAM} binary_dir)
install(FILES ${binary_dir}/shell/js
	DESTINATION /usr/local/bin/
	PERMISSIONS OWNER_READ OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE
	RENAME pjscan-js)

message(STATUS "Making pjscan-js - done!")
