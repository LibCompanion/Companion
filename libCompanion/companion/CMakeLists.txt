set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

unset(CUDA_USE_STATIC_CUDA_RUNTIME CACHE)
option(CUDA_USE_STATIC_CUDA_RUNTIME OFF)

SET(SOURCE
        companion.cpp companion.h
        openCV3/stream/Video.cpp openCV3/stream/Video.h
        openCV3/search/ImageRecognition.cpp openCV3/search/ImageRecognition.h
        openCV3/util/Util.cpp openCV3/util/Util.h
        openCV3/model/Comparison.cpp openCV3/model/Comparison.h
        openCV3/search/FeatureMatching.cpp openCV3/search/FeatureMatching.h
        openCV3/detection/HarrisCorner.cpp openCV3/detection/HarrisCorner.h
        openCV3/detection/ObjectTracking.cpp openCV3/detection/ObjectTracking.h
        openCV3/util/CompanionError.h
        openCV3/thread/ConsumerStream.cpp openCV3/thread/ConsumerStream.h
        openCV3/thread/ProducerStream.cpp openCV3/thread/ProducerStream.h
        )

add_library(libCompanion ${SOURCE})
INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIR})
target_link_libraries(libCompanion ${OpenCV_LIBS} ${Boost_LIBRARIES})