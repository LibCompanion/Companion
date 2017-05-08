# Create executable and set linked libraries
add_executable(Camshift camshift.cpp)
target_link_libraries(Camshift ${OpenCV_LIBS})


# Add target properties
set_property(TARGET Camshift PROPERTY FOLDER "libCompanion/testing")
