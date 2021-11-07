cmake_minimum_required(VERSION 3.5)
project(myengine VERSION 0.0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(external/glm)
add_subdirectory(external/glfw)
add_subdirectory(external/vk-bootstrap)

find_package(Vulkan REQUIRED)

message(STATUS "Found Vulkan" ${Vulkan_LIBRARIES})

file(GLOB_RECURSE SOURCES "src/*.cpp")

add_library(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME}
    PUBLIC
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        glm
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        ${Vulkan_INCLUDE_DIRS}
        glfw
        vk-bootstrap
)

target_link_libraries(${PROJECT_NAME}
    PUBLIC
        glm
    PRIVATE
        ${Vulkan_LIBRARIES}
        glfw
        vk-bootstrap
)