project(Companion)
cmake_minimum_required(VERSION 2.8)

set(CMAKE_CONFIGURATION_TYPES "Debug;Release")

find_package(OpenCV REQUIRED)
find_package(OpenMP REQUIRED)
find_package(Boost REQUIRED COMPONENTS thread system)

add_subdirectory(libCompanion)
add_subdirectory(main)
add_subdirectory(testing)
