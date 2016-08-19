set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

SET(SOURCE
  companion.cpp
  search/search.cpp
  search/compare.cpp
  search/flann.cpp
  search/templatematch.cpp
)

SET(HEADERS
  companion.h
  search/search.h
  search/compare.h
  search/flann.h
  search/templatematch.h
)

add_executable(${PROJECT_NAME} main.cpp ${SOURCE} ${HEADERS})
target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS})
