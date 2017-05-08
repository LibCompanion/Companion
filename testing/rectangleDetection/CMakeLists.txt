set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")


# Create executable and set linked libraries
add_executable(RectangleDetection RectangleDetection.cpp)
target_link_libraries(RectangleDetection ${OpenCV_LIBS})


# Add target properties
set_property(TARGET RectangleDetection PROPERTY FOLDER "libCompanion/testing")
