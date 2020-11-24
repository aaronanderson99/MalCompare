# Copyright 2012 Nedim Srndic, University of Tuebingen
# 
# This file is part of libpdfjs.
#
# libpdfjs is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
# 
# libpdfjs is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with libpdfjs.  If not, see <http://www.gnu.org/licenses/>.

# Build a test executable

# Source files
set(PDFJS_TEST_EXECUTABLE_NAME test)
set(PDFJS_TEST_EXECUTABLE_SOURCES test.cpp)

# Create the executable
add_executable(${PDFJS_TEST_EXECUTABLE_NAME} ${PDFJS_TEST_EXECUTABLE_SOURCES})
target_link_libraries(${PDFJS_TEST_EXECUTABLE_NAME} ${PDFJS_SHARED_LIBRARY_NAME})

# Set the executable version
set_target_properties(${PDFJS_TEST_EXECUTABLE_NAME} PROPERTIES VERSION ${PDFJS_VERSION})
