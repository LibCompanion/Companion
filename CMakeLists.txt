#
# This program is an image recognition library written with OpenCV.
# Copyright (C) 2016-2017 Andreas Sekulski
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
#

# Set installation directory for WIN32 (on UNIX '/usr/local' should be the default)
if(WIN32)
    set(CMAKE_INSTALL_PREFIX "${CMAKE_BINARY_DIR}/install" CACHE PATH "Installation Directory")
endif()


# Configure CMake project
project(Companion)


# Define CMake minimum version
if(WINDOWS_STORE)
    cmake_minimum_required(VERSION 3.4 FATAL_ERROR)
else()
    cmake_minimum_required(VERSION 2.8 FATAL_ERROR)
endif()


# Define CMake Flags
set(CMAKE_CONFIGURATION_TYPES "Debug;Release")
set(CMAKE_SKIP_INSTALL_ALL_DEPENDENCY ON)
set(CMAKE_DEBUG_POSTFIX "D")


# Define interactive CMake Flags
option(Companion_USE_CUDA "Use cuda implementation of libCompanion" OFF)
set(Companion_BUILD_SHARED_LIBRARY OFF CACHE BOOL "Build libCompanion as shared library")


# Define global properties
set_property(GLOBAL PROPERTY USE_FOLDERS ON)


# Configure dependencies
set(OpenCVComponents "core" "imgproc" "imgcodecs" "features2d" "videoio" "calib3d")
if(Companion_USE_CUDA)
    set(OpenCVComponents ${OpenCVComponents} "cudafeatures2d")
endif()
find_package(OpenCV REQUIRED ${OpenCVComponents})
find_package(OpenMP REQUIRED)


# Configure libCompanion
add_subdirectory(libCompanion)

# Configure additional modules
if(NOT WINDOWS_STORE)
    add_subdirectory(samples)
endif()
