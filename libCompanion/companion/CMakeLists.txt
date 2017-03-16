set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

option(USE_CUDA "Use cuda implementation from libCompanion" OFF)

SET(SOURCE
        Companion.cpp Companion.h
        algo/abstract/ImageRecognition.h
        algo/abstract/AbstractFeatureMatching.cpp algo/abstract/AbstractFeatureMatching.h
        algo/cpu/CPUFeatureMatching.cpp algo/cpu/CPUFeatureMatching.h
        draw/Drawable.h
        draw/Lines.cpp draw/Lines.h
        draw/Line.cpp draw/Line.h
        model/FeatureMatchingModel.cpp model/FeatureMatchingModel.h
        model/ImageRecognitionModel.cpp model/ImageRecognitionModel.h
        processing/ImageProcessing.h
        processing/2D/ObjectDetection.cpp processing/2D/ObjectDetection.h
        stream/Stream.h
        stream/Video.cpp stream/Video.h
        stream/Image.cpp stream/Image.h
        thread/StreamWorker.cpp thread/StreamWorker.h
        util/CompanionError.h
        util/Util.cpp util/Util.h
        util/Definitions.h)

if(USE_CUDA)
    SET(SOURCE ${SOURCE}
            algo/cuda/CudaFeatureMatching.cpp algo/cuda/CudaFeatureMatching.h)
endif()

add_library(Companion ${SOURCE})
target_link_libraries(Companion ${OpenCV_LIBS})