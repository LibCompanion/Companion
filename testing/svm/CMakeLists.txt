set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")


# Create executable and set linked libraries
add_executable(SVM svm.cpp)
target_link_libraries(SVM ${OpenCV_LIBS})


# Add target properties
set_property(TARGET SVM PROPERTY FOLDER "libCompanion/testing")
