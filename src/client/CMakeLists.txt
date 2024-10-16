cmake_minimum_required(VERSION 3.10)

project(client VERSION 1.0)

include_directories(include)

file(GLOB SOURCES "src/*.cpp" main.cpp)

add_executable(client ${SOURCES})

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)