cmake_minimum_required(VERSION 3.10)
project(EdgeQuest)

set(CMAKE_CXX_STANDARD 11)

# Specify the source files
set(SOURCES
    src/Edge-Quest.cpp
    src/main.cpp
)

# Add the executable
add_executable(EdgeQuest ${SOURCES})

# Include directories if needed
# target_include_directories(EdgeQuest PRIVATE include)

# Optionally, you can add additional configurations for WebAssembly here
# For example, if using Emscripten, you might want to set specific flags
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -s WASM=1")