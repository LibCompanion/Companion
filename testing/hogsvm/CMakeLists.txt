add_executable(HOG hog.cpp)
target_link_libraries(HOG ${OpenCV_LIBS})
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")