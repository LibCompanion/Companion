set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
unset(CUDA_USE_STATIC_CUDA_RUNTIME CACHE)
option(CUDA_USE_STATIC_CUDA_RUNTIME OFF)

SET(SOURCE
  companion.cpp
)

SET(HEADERS
  companion.h
)

add_subdirectory(search)
add_executable(${PROJECT_NAME} main.cpp ${SOURCE} ${HEADERS})
#add_library(${PROJECT_NAME} SHARED ${SOURCE})
target_link_libraries(${PROJECT_NAME} ${PROJECT_NAME}_Search ${OpenCV_LIBS})
