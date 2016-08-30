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

add_library(libCompanionSearch ${SOURCE} ${HEADERS})
target_include_directories (libCompanionSearch PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(libCompanionSearch ${OpenCV_LIBS})
