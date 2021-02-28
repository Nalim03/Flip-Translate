cmake_minimum_required(VERSION 3.16)

project(FlipTranslate)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

file(GLOB_RECURSE SRCS "src/*.cpp", "src/*.h")
add_executable(FlipTranslate ${SRCS})
target_include_directories(FlipTranslate PRIVATE ${CMAKE_SOURCE_DIR}/src)
