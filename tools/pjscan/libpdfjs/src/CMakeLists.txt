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

# Build and install the pdfjs libraries (static and shared)

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

require_file("poppler/Object.h")

set(PDFJS_SOURCES PdfJsExtractor.cpp)

# Create a shared and a static library
add_library(${PDFJS_SHARED_LIBRARY_NAME} SHARED ${PDFJS_SOURCES})
target_link_libraries(${PDFJS_SHARED_LIBRARY_NAME} ${PDFJS_REQUIRED_LIBRARIES})
add_library(${PDFJS_STATIC_LIBRARY_NAME} STATIC ${PDFJS_SOURCES})
target_link_libraries(${PDFJS_STATIC_LIBRARY_NAME} ${PDFJS_REQUIRED_LIBRARIES})

# Now we change the output name of "pdfjs-static.a" to "pdfjs.a"
# The library target "pdfjs" already has a default OUTPUT_NAME of "pdfjs", so we don't need to change it.
# The library target "pdfjs-static" has a default OUTPUT_NAME of "pdfjs-static", so change it.
set_target_properties(${PDFJS_STATIC_LIBRARY_NAME} 
    PROPERTIES OUTPUT_NAME ${PDFJS_SHARED_LIBRARY_NAME})
# Now the library target "pdfjs-static" will be named "pdfjs.lib" with MS tools.
# This conflicts with the "pdfjs.lib" import library corresponding to "pdfjs.dll",
# so we add a "lib" prefix (which is default on other platforms anyway):
set_target_properties(${PDFJS_STATIC_LIBRARY_NAME} 
    PROPERTIES PREFIX "lib")

# Set the library version
set_target_properties(${PDFJS_STATIC_LIBRARY_NAME} ${PDFJS_SHARED_LIBRARY_NAME}
    PROPERTIES VERSION ${PDFJS_VERSION})

# Install the libraries
install(TARGETS ${PDFJS_SHARED_LIBRARY_NAME} 
    LIBRARY DESTINATION lib
    PERMISSIONS OWNER_READ OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE)
install(TARGETS ${PDFJS_STATIC_LIBRARY_NAME} 
    ARCHIVE DESTINATION lib
    PERMISSIONS OWNER_READ OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE)
# This runs the 'ldconfig' command, because CMake doesn't
install(CODE "execute_process(COMMAND ldconfig)")
