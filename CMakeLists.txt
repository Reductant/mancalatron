cmake_minimum_required(VERSION 3.13)

project(mancalatron CXX)

add_compile_definitions("FMT_HEADER_ONLY")

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()


# put all your source files here:
set(sources 
    src/mancalatron_v4.cpp
    src/utils.cpp
    src/gamestate.cpp
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD_REQUIRED OFF)

add_executable(mancalatron ${sources})
target_link_libraries(mancalatron ${CONAN_LIBS})
