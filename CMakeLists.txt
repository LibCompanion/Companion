project(Companion)
cmake_minimum_required(VERSION 2.8)

find_package(OpenCV REQUIRED)
find_package(OpenMP REQUIRED)

set(CMAKE_CONFIGURATION_TYPES "Debug;Release")
unset(CUDA_USE_STATIC_CUDA_RUNTIME CACHE)

option(CUDA_USE_STATIC_CUDA_RUNTIME OFF)

# Lib dependency
set(OpenCV_LIBS opencv_core opencv_imgproc opencv_highgui opencv_features2d)

add_subdirectory(libCompanion)
add_subdirectory(main)
