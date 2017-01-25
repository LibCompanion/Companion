set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

SET(SOURCE
        openCV3/algo/ImageRecognition.h
        openCV3/algo/FeatureMatching.cpp openCV3/algo/FeatureMatching.h
        openCV3/algo/RectangleDetection.cpp openCV3/algo/RectangleDetection.h
        openCV3/configuration/CompanionConfig.cpp openCV3/configuration/CompanionConfig.h
        openCV3/draw/Drawable.h
        openCV3/draw/Lines.cpp openCV3/draw/Lines.h
        openCV3/draw/Line.cpp openCV3/draw/Line.h
        openCV3/model/FeatureMatchingModel.cpp openCV3/model/FeatureMatchingModel.h
        openCV3/model/ImageRecognitionModel.cpp openCV3/model/ImageRecognitionModel.h
        openCV3/processing/ImageProcessing.h
        openCV3/processing/ObjectDetection.cpp openCV3/processing/ObjectDetection.h
        openCV3/stream/Video.cpp openCV3/stream/Video.h
        openCV3/thread/ConsumerStream.cpp openCV3/thread/ConsumerStream.h
        openCV3/thread/ProducerStream.cpp openCV3/thread/ProducerStream.h
        openCV3/util/CompanionError.h
        openCV3/util/Util.cpp openCV3/util/Util.h)

add_library(Companion ${SOURCE})
INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIR})
target_link_libraries(Companion ${OpenCV_LIBS} ${Boost_LIBRARIES})