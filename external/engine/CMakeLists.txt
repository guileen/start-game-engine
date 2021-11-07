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

# SHADER COMPILATION

file (GLOB SHADERS shaders/*.frag shaders/*.vert)

add_custom_command(TARGET ${PROJECT_NAME} PRE_BUILD
 COMMAND mkdir -p ${CMAKE_BINARY_DIR}/shaders)

foreach(shader ${SHADERS})
    get_filename_component(SHADER_NAME ${shader} NAME)
    message(STATUS "Compiling ${SHADER_NAME} -c ${shader}")
    add_custom_command(TARGET ${PROJECT_NAME}
        PRE_BUILD COMMAND glslc -o ${CMAKE_BINARY_DIR}/shaders/${SHADER_NAME}.spv -c ${shader})
endforeach()