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

# Build and install the svm_oc libraries (static and shared)

# Source files
set(SVM_OC_SOURCES libsvm.cpp SvmOc.cpp)

# Create a shared and a static library
add_library(${SVM_OC_SHARED_LIBRARY_NAME} SHARED ${SVM_OC_SOURCES})
target_link_libraries(${SVM_OC_SHARED_LIBRARY_NAME} ${SVM_OC_REQUIRED_LIBRARIES})
add_library(${SVM_OC_STATIC_LIBRARY_NAME} STATIC ${SVM_OC_SOURCES})
target_link_libraries(${SVM_OC_STATIC_LIBRARY_NAME} ${SVM_OC_REQUIRED_LIBRARIES})

# Now we change the output name of "svm_oc-static.a" to "svm_oc.a"
# The library target "svm_oc" already has a default OUTPUT_NAME of "svm_oc", so we don't need to change it.
# The library target "svm_oc-static" has a default OUTPUT_NAME of "svm_oc-static", so change it.
set_target_properties(${SVM_OC_STATIC_LIBRARY_NAME} 
    PROPERTIES OUTPUT_NAME ${SVM_OC_SHARED_LIBRARY_NAME})
# Now the library target "svm_oc-static" will be named "svm_oc.lib" with MS tools.
# This conflicts with the "svm_oc.lib" import library corresponding to "svm_oc.dll",
# so we add a "lib" prefix (which is default on other platforms anyway):
set_target_properties(${SVM_OC_STATIC_LIBRARY_NAME} 
    PROPERTIES PREFIX "lib")

# Set the library version
set_target_properties(${SVM_OC_STATIC_LIBRARY_NAME} ${SVM_OC_SHARED_LIBRARY_NAME}
    PROPERTIES VERSION ${SVM_OC_VERSION})

# Install the libraries
install(TARGETS ${SVM_OC_SHARED_LIBRARY_NAME} 
    LIBRARY DESTINATION lib
    PERMISSIONS OWNER_READ OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE)
install(TARGETS ${SVM_OC_STATIC_LIBRARY_NAME} 
    ARCHIVE DESTINATION lib
    PERMISSIONS OWNER_READ OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE)
# This runs the 'ldconfig' command, because CMake doesn't
install(CODE "execute_process(COMMAND ldconfig)")