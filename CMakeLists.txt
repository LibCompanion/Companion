project(Companion)
cmake_minimum_required(VERSION 2.8)

set(CMAKE_CONFIGURATION_TYPES "Debug;Release")

find_package(OpenCV REQUIRED)
find_package(OpenMP REQUIRED)
find_package(Boost REQUIRED COMPONENTS thread)

# Lib dependency
set(OpenCV_LIBS
        opencv_calib3d
        opencv_core
        opencv_features2d
        opencv_flann
        opencv_highgui
        opencv_imgcodecs
        opencv_imgproc
        opencv_ml
        opencv_objdetect
        opencv_photo
        opencv_shape
        opencv_stitching
        opencv_superres
        opencv_ts
        opencv_video
        opencv_videoio
        opencv_videostab)

add_subdirectory(libCompanion)
add_subdirectory(main)
