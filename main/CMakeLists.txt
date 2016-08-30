SET(SOURCE
  main.cpp
)

add_executable(Example ${SOURCE})
target_link_libraries(Example libCompanion)
