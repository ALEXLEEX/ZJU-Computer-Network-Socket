
cmake_minimum_required(VERSION 3.10)

project(client VERSION 1.0)

find_package(OpenSSL REQUIRED)


include_directories(include)
include_directories(../packet/include)

file(GLOB SOURCES "src/*.cpp" main.cpp "../packet/src/*.cpp")

add_executable(client ${SOURCES})

target_link_libraries(client OpenSSL::SSL OpenSSL::Crypto)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/../../bin)