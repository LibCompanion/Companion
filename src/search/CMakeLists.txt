SET(SOURCE
  search.cpp
  compare.cpp
  flann.cpp
  templatematch.cpp
)

SET(HEADERS
  search.h
  compare.h
  flann.h
  templatematch.h
)

add_library(${PROJECT_NAME}_Search ${SOURCE})
target_link_libraries(${PROJECT_NAME}_Search ${OpenCV_LIBS})