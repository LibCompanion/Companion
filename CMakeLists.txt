project(Companion)
cmake_minimum_required(VERSION 2.8)

set(CMAKE_CONFIGURATION_TYPES "Debug;Release")

find_package(Boost REQUIRED COMPONENTS thread system date_time chrono)
find_package(OpenCV REQUIRED COMPONENTS core imgproc features2d calib3d tracking)
find_package(OpenMP REQUIRED)

add_subdirectory(libCompanion)
add_subdirectory(main)
add_subdirectory(testing)
