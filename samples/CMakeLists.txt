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


# Create executable and set linked libraries
add_executable(2DObjectDetection 2DObjectDetection.cpp)
target_link_libraries(2DObjectDetection Companion)


# Add target properties
set_property(TARGET 2DObjectDetection PROPERTY FOLDER "libCompanion")


# add install instructions (also add debug postfix for differentiation)
set_property(TARGET 2DObjectDetection PROPERTY DEBUG_POSTFIX "D")
install(TARGETS 2DObjectDetection DESTINATION "${PROJECT_BINARY_DIR}/install/bin" OPTIONAL)
