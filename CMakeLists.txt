cmake_minimum_required(VERSION 3.16)

project(FlipTranslate)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

if(CMAKE_VERSION VERSION_LESS "3.7.0")
    set(CMAKE_INCLUDE_CURRENT_DIR ON)
endif()

find_package(Qt6 COMPONENTS Widgets REQUIRED)

include(FetchContent)

set(SQLITECPP_RUN_CPPLINT OFF CACHE INTERNAL "")
FetchContent_Declare(SQLiteCpp
    GIT_REPOSITORY https://github.com/SRombauts/SQLiteCpp
    GIT_TAG 3.1.1
)
FetchContent_MakeAvailable(SQLiteCpp)

file(GLOB_RECURSE SRCS "src/*.cpp" "src/*.h")
add_executable(FlipTranslate ${SRCS})
target_include_directories(FlipTranslate PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(FlipTranslate Qt6::Widgets SQLiteCpp sqlite3)
