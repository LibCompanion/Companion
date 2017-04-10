add_executable(RectangleDetection RectangleDetection.cpp)
target_link_libraries(RectangleDetection ${OpenCV_LIBS})
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")