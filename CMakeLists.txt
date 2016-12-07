project(Companion)
cmake_minimum_required(VERSION 2.8)

find_package(OpenCV REQUIRED)
find_package(OpenMP REQUIRED)

set(CMAKE_CONFIGURATION_TYPES "Debug;Release")
unset(CUDA_USE_STATIC_CUDA_RUNTIME CACHE)

option(CUDA_USE_STATIC_CUDA_RUNTIME OFF)

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
        opencv_videostab
        opencv_xfeatures2d)

add_subdirectory(libCompanion)
add_subdirectory(main)
