# Use Universal Windows Platform as target system
set(CMAKE_SYSTEM_NAME "WindowsStore")
set(CMAKE_SYSTEM_VERSION "10.0")

# Consume Windows Runtime Extension
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /ZW")

# Eliminate deprecation errors
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /D _CRT_SECURE_NO_WARNINGS")