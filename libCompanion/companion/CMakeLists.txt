set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

SET(SOURCE
        Companion.cpp Companion.h
        algo/ImageRecognition.h
        algo/FeatureMatching.cpp algo/FeatureMatching.h
        draw/Drawable.h
        draw/Lines.cpp draw/Lines.h
        draw/Line.cpp draw/Line.h
        model/FeatureMatchingModel.cpp model/FeatureMatchingModel.h
        model/ImageRecognitionModel.cpp model/ImageRecognitionModel.h
        processing/ImageProcessing.h
        processing/ObjectDetection.cpp processing/ObjectDetection.h
        stream/Video.cpp stream/Video.h
        thread/ConsumerStream.cpp thread/ConsumerStream.h
        thread/ProducerStream.cpp thread/ProducerStream.h
        util/CompanionError.h
        util/Util.cpp util/Util.h)

add_library(Companion ${SOURCE})
INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIR})
target_link_libraries(Companion ${OpenCV_LIBS} ${Boost_LIBRARIES})