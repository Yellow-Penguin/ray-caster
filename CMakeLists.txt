cmake_minimum_required(VERSION 3.23)
set(PROJECT_NAME ray-caster)
project(${PROJECT_NAME} C)

set(CMAKE_C_STANDARD 23)

set(SOURCE_FILES main.c include/glad/glad.c)
add_executable(${PROJECT_NAME} ${SOURCE_FILES})

add_subdirectory(include/GLFW)
target_link_libraries(${PROJECT_NAME} glfw)

add_subdirectory(include/glad)
target_link_libraries(${PROJECT_NAME} glad)
