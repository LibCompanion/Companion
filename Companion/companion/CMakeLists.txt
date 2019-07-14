#
# This program is an object recognition framework written with OpenCV.
# Copyright (C) 2016-2018 Andreas Sekulski, Dimitri Kotlovsky
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

if(UNIX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
endif()

# Add source files
set(SOURCE
    Configuration.cpp Configuration.h
    algo/detection/Detection.h
    algo/detection/ShapeDetection.cpp algo/detection/ShapeDetection.h
    algo/recognition/Recognition.h
    algo/recognition/hashing/Hashing.h 
    algo/recognition/hashing/LSH.cpp algo/recognition/hashing/LSH.h
    algo/recognition/matching/Matching.h
    algo/recognition/matching/FeatureMatching.cpp algo/recognition/matching/FeatureMatching.h
    algo/recognition/matching/util/IRA.cpp algo/recognition/matching/util/IRA.h
    draw/Drawable.h
    draw/Frame.cpp draw/Frame.h
    draw/Line.cpp draw/Line.h
    input/Stream.h
    input/Video.cpp input/Video.h
    input/Image.cpp input/Image.h
    model/result/Result.h model/result/Result.cpp
    model/result/DetectionResult.cpp model/result/DetectionResult.h
    model/result/RecognitionResult.cpp model/result/RecognitionResult.h
    model/processing/FeatureMatchingModel.cpp model/processing/FeatureMatchingModel.h
    model/processing/ImageHashModel.cpp model/processing/ImageHashModel.h
    processing/ImageProcessing.h
    processing/detection/ObjectDetection.cpp processing/detection/ObjectDetection.h
    processing/recognition/MatchRecognition.cpp processing/recognition/MatchRecognition.h
    processing/recognition/HashRecognition.cpp processing/recognition/HashRecognition.h
    processing/recognition/HybridRecognition.cpp processing/recognition/HybridRecognition.h
    thread/StreamWorker.cpp thread/StreamWorker.h
    util/CompanionError.h
    util/Util.cpp util/Util.h
    util/Definitions.h
    util/exportapi/ExportAPIDefinitions.h
    util/CompanionException.cpp util/CompanionException.h)

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
if(UNIX)
    target_link_libraries(Companion -lpthread)
endif()

# Add target properties
set_property(TARGET Companion PROPERTY FOLDER "Companion")

# add install instructions
install(TARGETS Companion EXPORT CompanionConfig
        RUNTIME DESTINATION "bin"
        LIBRARY DESTINATION "lib"
        ARCHIVE DESTINATION "lib")
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} DESTINATION include FILES_MATCHING PATTERN "*.h")
install(EXPORT CompanionConfig DESTINATION "lib")
