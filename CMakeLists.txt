project(Companion)
cmake_minimum_required(VERSION 2.8)

set(CMAKE_CONFIGURATION_TYPES "Debug;Release")

option(BUILD_UWP "Build lib for UWP" OFF)

if(BUILD_UWP)
    set(CMAKE_SYSTEM_NAME "WindowsStore" CACHE FILEPATH "Windows Environment Build like Windows Store")
	set(CMAKE_SYSTEM_VERSION "10" CACHE FILEPATH "Prefered Windows Version Build")
endif()

find_package(OpenCV REQUIRED)
find_package(OpenMP REQUIRED)

add_subdirectory(libCompanion)
add_subdirectory(main)
add_subdirectory(testing)

