cmake_minimum_required(VERSION 3.29)
project(Vast)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(Vast
    cvast/main.cpp
        cvast/headers/PythonTokenizer.h)

include_directories(/opt/homebrew/opt/python@3.13/Frameworks/Python.framework/Versions/3.13/include/python3.13)

target_link_libraries(${PROJECT_NAME}
        /opt/homebrew/opt/python@3.13/Frameworks/Python.framework/Versions/3.13/lib/libpython3.13.dylib
)

target_include_directories(Vast PRIVATE /opt/homebrew/include)

