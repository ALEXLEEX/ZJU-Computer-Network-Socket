cmake_minimum_required(VERSION 3.10)

project(server VERSION 2.0 LANGUAGES CXX DESCRIPTION "Server for ZJU-Computer-Network-Socket")

set(CMAKE_CXX_STANDARD 11)

# Source files.
file(GLOB SOURCES
    src/*.cpp
    ../packet/src/*.cpp
)

include_directories(
    include/
    ../packet/include/
)

# Add dependencies.
find_package(OpenSSL REQUIRED)

# Add executable target.
add_executable(SERVER main.cpp)

add_library(server ${SOURCES})

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

# Link dependencies.
target_link_libraries(SERVER OpenSSL::SSL OpenSSL::Crypto server)