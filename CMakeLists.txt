cmake_minimum_required(VERSION 3.10)
include(FindPackageHandleStandardArgs)

project(YieArKungFu)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH};${CMAKE_SOURCE_DIR}")
set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_BUILD_TYPE RelWithDebInfo)
add_compile_options(-O0 -v)

find_package(raylib REQUIRED)

file(GLOB SOURCES "src/*.cpp")

include_directories(
    ${raylib_INCLUDE_DIRS}
    ${PROJECT_SOURCE_DIR}/include
)

link_libraries(
    ${raylib_LIBRARIES}
)

add_executable(kungfu ${SOURCES})