cmake_minimum_required(VERSION 3.20)
project(ImageSteganography)

set(CMAKE_CXX_STANDARD 20)

include_directories(assets)
file(COPY assets DESTINATION ${CMAKE_BINARY_DIR})

add_executable(${PROJECT_NAME} main.cpp ImageSteganography.h ImageSteganography.cpp)
