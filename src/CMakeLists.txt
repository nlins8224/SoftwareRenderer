cmake_minimum_required(VERSION 3.10)
project(renderer)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if ( CMAKE_COMPILER_IS_GNUCC )
    set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic")
endif()

add_subdirectory(Math)
add_subdirectory(Shaders)

set(renderer_source_files main.cpp gl.cpp gl.h model.cpp model.h tgaimage.cpp tgaimage.h)
add_executable(${PROJECT_NAME} ${renderer_source_files})

target_link_libraries(renderer math shaders)
