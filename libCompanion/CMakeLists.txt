add_subdirectory(companion)
target_include_directories (Companion PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>  
    $<INSTALL_INTERFACE:include>  # <prefix>/include
)
