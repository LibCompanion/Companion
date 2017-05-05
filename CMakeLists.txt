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
option(USE_CUDA "Use cuda implementation from libCompanion" OFF)
if(WINDOWS_STORE)
    set(BUILD_SHARED_LIBS ON)
else()
    set(BUILD_SHARED_LIBS OFF CACHE BOOL "Build shared library")
endif()


# Define global properties
set_property(GLOBAL PROPERTY USE_FOLDERS ON)


# Configure dependencies
set(OpenCVComponents "core" "imgproc" "imgcodecs" "features2d" "videoio" "calib3d")
if(USE_CUDA)
    set(OpenCVComponents ${OpenCVComponents} "cudafeatures2d")
endif()
find_package(OpenCV REQUIRED ${OpenCVComponents})
find_package(OpenMP REQUIRED)


# Configure libCompanion
add_subdirectory(libCompanion)


# Configure additional modules
if(NOT WINDOWS_STORE)
    add_subdirectory(main)
    add_subdirectory(testing)
endif()
