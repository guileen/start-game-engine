cmake_minimum_required(VERSION 3.5)
project(youtube_cpp_application VERSION 0.0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(external/engine)

add_executable(${PROJECT_NAME} src/main.cpp)

target_link_libraries(${PROJECT_NAME} myengine)
target_include_directories(${PROJECT_NAME} PRIVATE myengine)