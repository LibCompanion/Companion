add_executable(SVM svm.cpp)
target_link_libraries(SVM ${OpenCV_LIBS})
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")