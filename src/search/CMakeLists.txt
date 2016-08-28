SET(SOURCE
  search.cpp
  compare.cpp
  featurematch.cpp
  templatematch.cpp
)

SET(HEADERS
  search.h
  compare.h
  featurematch.h
  templatematch.h
)

add_library(${PROJECT_NAME}_Search ${SOURCE} ${HEADERS})
target_link_libraries(${PROJECT_NAME}_Search ${OpenCV_LIBS})