cmake_minimum_required(VERSION 3.5)
project(demo_game VERSION 0.0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
# 编译选项
if(MSVC)
  add_compile_options( /W4 /WX)
else()
  #  add_compile_options (-fdiagnostics-color=always) #GNU
  add_compile_options(-w -fcolor-diagnostics) #Clang
endif()

add_subdirectory(engine)

add_executable(${PROJECT_NAME} src/main.cpp)

target_link_libraries(${PROJECT_NAME} myengine)
target_include_directories(${PROJECT_NAME} PRIVATE myengine)