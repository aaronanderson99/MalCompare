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

# Build and install the stem libraries (static and shared)

# A macro for aborting if required include files are not found
macro(require_file RF_FILE_NAME)
    find_file(RF_INCLUDE_H ${RF_FILE_NAME})
	if (RF_INCLUDE_H)
    	message(STATUS "Found header file ${RF_FILE_NAME}")
    else (RF_INCLUDE_H)
    	message(FATAL_ERROR "Missing header file ${RF_FILE_NAME}")
	endif (RF_INCLUDE_H)
	unset(RF_INCLUDE_H CACHE)
	unset(RF_INCLUDE_H)
endmacro(require_file)

# Some include files must be present
require_file("boost/shared_ptr.hpp")
require_file("boost/shared_array.hpp")

# Source files
set(STEM_SOURCES NGram.cpp NGramConf.cpp NGramItem.cpp Kernel.cpp util.cpp)

# Create a shared and a static library
add_library(${STEM_SHARED_LIBRARY_NAME} SHARED ${STEM_SOURCES})
add_library(${STEM_STATIC_LIBRARY_NAME} STATIC ${STEM_SOURCES})

# Now we change the output name of "stem-static.a" to "stem.a"
# The library target "stem" already has a default OUTPUT_NAME of "stem", so we don't need to change it.
# The library target "stem-static" has a default OUTPUT_NAME of "stem-static", so change it.
set_target_properties(${STEM_STATIC_LIBRARY_NAME} 
    PROPERTIES OUTPUT_NAME ${STEM_SHARED_LIBRARY_NAME})
# Now the library target "stem-static" will be named "stem.lib" with MS tools.
# This conflicts with the "stem.lib" import library corresponding to "stem.dll",
# so we add a "lib" prefix (which is default on other platforms anyway):
set_target_properties(${STEM_STATIC_LIBRARY_NAME} 
    PROPERTIES PREFIX "lib")

# Set the library version
set_target_properties(${STEM_STATIC_LIBRARY_NAME} ${STEM_SHARED_LIBRARY_NAME}
    PROPERTIES VERSION ${STEM_VERSION})

# Install the libraries
install(TARGETS ${STEM_SHARED_LIBRARY_NAME} 
    LIBRARY DESTINATION lib
    PERMISSIONS OWNER_READ OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE)
install(TARGETS ${STEM_STATIC_LIBRARY_NAME} 
    ARCHIVE DESTINATION lib
    PERMISSIONS OWNER_READ OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE)
# This runs the 'ldconfig' command, because CMake doesn't
install(CODE "execute_process(COMMAND ldconfig)")
