#
# This program is an object recognition framework written with OpenCV.
# Copyright (C) 2016-2018 Andreas Sekulski, Dimitri Kotlovsky
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

IF(NOT CMAKE_BUILD_TYPE)
    SET(CMAKE_BUILD_TYPE Release)
ENDIF(NOT CMAKE_BUILD_TYPE)

# Define CMake Flags
set(CMAKE_CONFIGURATION_TYPES "Debug;Release")
set(CMAKE_SKIP_INSTALL_ALL_DEPENDENCY ON)
set(CMAKE_DEBUG_POSTFIX "D")

# Define interactive CMake Flags
option(Companion_BUILD_SHARED_LIBRARY "Build Companion as shared library" OFF)

# Cuda and current samples are not supported when building for Windows Store
if(NOT WINDOWS_STORE)
    option(Companion_BUILD_SAMPLES "Build all Companion samples" OFF)
    option(Companion_USE_CUDA "Use cuda implementation of Companion" OFF)
    option(Companion_USE_XFEATURES_2D "Use non free module" OFF)
endif()

# Define global properties
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# Configure dependencies
set(OpenCVComponents "core" "imgproc" "imgcodecs" "features2d" "videoio" "calib3d")
if(Companion_USE_CUDA)
    set(OpenCVComponents ${OpenCVComponents} "cudafeatures2d")
    add_definitions(-DCompanion_USE_CUDA)
endif()

if(Companion_USE_XFEATURES_2D)
    set(OpenCVComponents ${OpenCVComponents} "xfeatures2d")
endif()

find_package(OpenCV REQUIRED ${OpenCVComponents})
find_package(OpenMP REQUIRED)
if(OPENMP_FOUND)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
endif()

# For developer
if(Companion_DEBUG)
    add_definitions(-DCompanion_DEBUG)
endif()

# Configure Companion
add_subdirectory(Companion)

# Configure to build additional modules
if(Companion_BUILD_SAMPLES)
	if(EXISTS "${PROJECT_SOURCE_DIR}/CompanionSamples/CMakeLists.txt")
		add_subdirectory(CompanionSamples)
	else()
		set(Companion_SAMPLE_MODULE "Path_to_Samples_Module" CACHE PATH "Sample module path")
        add_subdirectory(${Companion_SAMPLE_MODULE} samples)
	endif()
endif()