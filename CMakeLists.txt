project(Companion)
cmake_minimum_required(VERSION 2.8)

set(CMAKE_CONFIGURATION_TYPES "Debug;Release")

option(USE_CUDA "Use cuda implementation from libCompanion" OFF)
option(BUILD_SHARED_LIBRARY "Build shared library" OFF)

set(OpenCVComponents "core" "imgproc" "imgcodecs" "features2d" "videoio" "calib3d")
if(USE_CUDA)
    set(OpenCVComponents ${OpenCVComponents} "cudafeatures2d")
endif()

find_package(OpenCV REQUIRED ${OpenCVComponents})
find_package(OpenMP REQUIRED)

add_subdirectory(libCompanion)
add_subdirectory(main)
add_subdirectory(testing)

