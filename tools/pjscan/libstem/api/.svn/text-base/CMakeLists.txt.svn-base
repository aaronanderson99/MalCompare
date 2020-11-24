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

# Build a Python API for stem using SWIG

# SWIG is required
find_package(SWIG REQUIRED)
include(${SWIG_USE_FILE})

# Python is required
find_package(PythonLibs)
include_directories(${PYTHON_INCLUDE_PATH})

# Find out Python installation directory
execute_process(
    COMMAND 
    python -c "from distutils.sysconfig import get_python_lib; print get_python_lib().split('/')[-2] + '/' + get_python_lib().split('/')[-1]" 
    OUTPUT_VARIABLE PYTHON_SITE_PACKAGES 
    OUTPUT_STRIP_TRAILING_WHITESPACE
)

# Set things up for SWIG
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
set(CMAKE_SWIG_FLAGS "")
set(PYTHON_SITE_PACKAGES "lib/${PYTHON_SITE_PACKAGES}")

# Set up variable values
set(MODULE_NAME stem)
set(MODULE_FILE_NAME stem.i)
set(MODULE_CPP_FILE_NAME stem_wrap.cxx)
set(MODULE_TARGET_NAME stem.py)
set(MODULE_TARGET_LIBRARY_NAME _stem.so)

# Set up and run SWIG
set_source_files_properties(${MODULE_FILE_NAME} PROPERTIES CPLUSPLUS ON)
swig_add_module(${MODULE_NAME} python ${MODULE_FILE_NAME})
swig_link_libraries(${MODULE_NAME} 
    ${PYTHON_LIBRARIES} ${STEM_SHARED_LIBRARY_NAME})

# Install the python module
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/${MODULE_TARGET_NAME}" 
    DESTINATION ${PYTHON_SITE_PACKAGES}
    PERMISSIONS OWNER_READ OWNER_WRITE GROUP_READ WORLD_READ)
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/${MODULE_TARGET_LIBRARY_NAME}" 
    DESTINATION ${PYTHON_SITE_PACKAGES}
    PERMISSIONS OWNER_READ OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE)