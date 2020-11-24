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

# Build and install the pjscan executables (parent and child)

# A macro for aborting if a required include file is not found
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
require_file("boost/thread.hpp")
require_file("pdfjs/PdfJsExtractor.h")
require_file("pdfjs/PdfJsExtractor.h")
require_file("stem/NGram.h")
require_file("svm_oc/SvmOc.h")
require_file("signal.h")
require_file("sys/stat.h")
require_file("sys/time.h")
require_file("sys/types.h")
require_file("sys/wait.h")
require_file("fcntl.h")
require_file("unistd.h")
require_file("errno.h")
require_file("boost/filesystem.hpp")
require_file("boost/date_time.hpp")
require_file("argtable2.h")

# Source files
set(PJSCAN_SOURCES JsDumper.cpp WorkerThread.cpp Learner.cpp Classifier.cpp ThreadSafeMap.cpp ThreadPool.cpp timing.cpp FeaturePrinter.cpp tokens.cpp pjscan.cpp)
set(PJSCAN_CHILD_SOURCES pjscan-child.cpp timing.cpp)

# Create a parent and a child executable
add_executable(${PJSCAN_EXECUTABLE_NAME} ${PJSCAN_SOURCES})
target_link_libraries(${PJSCAN_EXECUTABLE_NAME} ${PJSCAN_REQUIRED_LIBRARIES})
add_executable(${PJSCAN_CHILD_EXECUTABLE_NAME} ${PJSCAN_CHILD_SOURCES})
target_link_libraries(${PJSCAN_CHILD_EXECUTABLE_NAME} ${PJSCAN_CHILD_REQUIRED_LIBRARIES})

# Set the executable version
set_target_properties(${PJSCAN_EXECUTABLE_NAME} ${PJSCAN_CHILD_EXECUTABLE_NAME}
    PROPERTIES VERSION ${PJSCAN_VERSION})

# Install the executables
install(TARGETS ${PJSCAN_EXECUTABLE_NAME} 
    RUNTIME DESTINATION bin
    PERMISSIONS OWNER_READ OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE)
install(TARGETS ${PJSCAN_CHILD_EXECUTABLE_NAME} 
    RUNTIME DESTINATION bin
    PERMISSIONS OWNER_READ OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE)
