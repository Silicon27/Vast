cmake_minimum_required(VERSION 3.29)
project(Vast)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Use the new find_package for Python 3
find_package(Python3 REQUIRED)

# Include the Python headers found by find_package
include_directories(${Python3_INCLUDE_DIRS})

# Create the executable
add_executable(Vast
        cvast/main.cpp
        cvast/headers/PythonTokenizer.h
)

# Link Python libraries to the executable
target_link_libraries(Vast ${Python3_LIBRARIES})

# If you need additional include directories, you can add them here
target_include_directories(Vast PRIVATE /opt/homebrew/include)
