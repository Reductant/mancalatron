cmake_minimum_required(VERSION 3.13)

project(mancalatron CXX)

# put all your source files here:
set(sources 
    src/mancalatron_v4.cpp
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD_REQUIRED OFF)

add_executable(mancalatron ${sources})
