cmake_minimum_required(VERSION 3.10)

project(server VERSION 2.0 DESCRIPTION "Server for ZJU-Computer-Network-Socket")

set(CMAKE_CXX_STANDARD 11)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/../../bin)

# Add dependencies.
find_package(OpenSSL REQUIRED)

# Include directories.
include_directories(
    include/
    ../packet/include/
)

# Source files.
file(GLOB SOURCES
    "src/*.cpp"
    "../packet/src/*.cpp"
    main.cpp
)

# Add executable target.
add_executable(server ${SOURCES})

# Link dependencies.
target_link_libraries(server OpenSSL::SSL OpenSSL::Crypto)