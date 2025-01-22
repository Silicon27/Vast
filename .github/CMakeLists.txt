cmake_minimum_required(VERSION 3.29)
project(Vast)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include the Python headers from the correct path
include_directories(/usr/include/python3.13)

# Create the executable
add_executable(Vast
        cvast/main.cpp
        cvast/headers/PythonTokenizer.h
)

# Link Python library (ensure it's the correct version)
target_link_libraries(${PROJECT_NAME}
        /usr/lib/python3.13/config-3.13-x86_64-linux-gnu/libpython3.13.so
)

# Add the include directories for other necessary libraries if needed
target_include_directories(Vast PRIVATE /opt/homebrew/include)
