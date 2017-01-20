add_executable(camshift camshift.cpp)
INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIR})
target_link_libraries(camshift ${OpenCV_LIBS} ${Boost_LIBRARIES})