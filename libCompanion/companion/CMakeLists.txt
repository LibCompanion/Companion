#
# This program is an image recognition library written with OpenCV.
# Copyright (C) 2016-2017 Andreas Sekulski
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
#

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

# Add source files
SET(SOURCE
        Configuration.cpp Configuration.h
        algo/abstract/ImageRecognition.h
        algo/abstract/AbstractFeatureMatching.cpp algo/abstract/AbstractFeatureMatching.h
        algo/cpu/CPUFeatureMatching.cpp algo/cpu/CPUFeatureMatching.h
        algo/util/IRA.cpp algo/util/IRA.h
        draw/Drawable.h
        draw/Lines.cpp draw/Lines.h
        draw/Line.cpp draw/Line.h
        model/FeatureMatchingModel.cpp model/FeatureMatchingModel.h
        model/ImageRecognitionModel.cpp model/ImageRecognitionModel.h
        processing/ImageProcessing.h
        processing/2D/ObjectDetection.cpp processing/2D/ObjectDetection.h
        input/Stream.h
        input/Video.cpp input/Video.h
        input/Image.cpp input/Image.h
        thread/StreamWorker.cpp thread/StreamWorker.h
        util/CompanionError.h
        util/Util.cpp util/Util.h
        util/Definitions.h
        util/exportapi/ExportAPIDefinitions.h)
if(Companion_USE_CUDA)
    SET(SOURCE ${SOURCE}
            algo/cuda/CudaFeatureMatching.cpp algo/cuda/CudaFeatureMatching.h)
endif()


# Create MSVC filter
if(MSVC)
    foreach(source IN LISTS SOURCE)
        get_filename_component(source_path "${source}" PATH)
        string(REPLACE "/" "\\" source_path_msvc "${source_path}")
        source_group("${source_path_msvc}" FILES "${source}")
    endforeach()
endif()


# Create library and set linked libraries
if(Companion_BUILD_SHARED_LIBRARY)
    add_library(Companion SHARED ${SOURCE})
else()
    add_library(Companion ${SOURCE})
endif()
target_link_libraries(Companion ${OpenCV_LIBS})


# Add target properties
set_property(TARGET Companion PROPERTY FOLDER "libCompanion")


# add install instructions
install(TARGETS Companion EXPORT CompanionConfig
        RUNTIME DESTINATION "bin"
        LIBRARY DESTINATION "lib"
        ARCHIVE DESTINATION "lib")
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} DESTINATION include FILES_MATCHING PATTERN "*.h")
install(EXPORT CompanionConfig DESTINATION "lib")
