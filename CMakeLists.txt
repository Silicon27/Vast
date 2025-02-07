cmake_minimum_required(VERSION 3.29)
project(Vast)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#find_package(LLVM REQUIRED CONFIG)
#include_directories(${LLVM_INCLUDE_DIRS})
#add_definitions(${LLVM_DEFINITIONS})

add_executable(Vast
    cvast/main.cpp
)

find_package(LLVM REQUIRED CONFIG)

include_directories(${LLVM_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS})

set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -L/opt/homebrew/opt/llvm/lib -Wl,-rpath,/opt/homebrew/opt/llvm/lib -lLLVM")

target_link_libraries(Vast PRIVATE ${LLVM_LIBRARIES})
target_link_libraries(Vast PRIVATE LLVMTargetParser)

#target_link_libraries(Vast PRIVATE ${LLVM_LIBRARIES})

include_directories(/opt/homebrew/opt/python@3.13/Frameworks/Python.framework/Versions/3.13/include/python3.13)

include_directories(/opt/homebrew/opt/llvm/include)

target_link_libraries(Vast PRIVATE
        /opt/homebrew/opt/python@3.13/Frameworks/Python.framework/Versions/3.13/lib/libpython3.13.dylib
)

target_include_directories(Vast PRIVATE /opt/homebrew/include)

