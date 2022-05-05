cmake_minimum_required(VERSION 3.10.0)
project(OpenGL_Tutorial VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(PROJECT_ROOT ${PROJECT_SOURCE_DIR})

file(COPY ${PROJECT_ROOT}/res DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/)

add_subdirectory(${PROJECT_ROOT}/externals/glfw-3.3.6/)
add_subdirectory(${PROJECT_ROOT}/externals/glew-2.1.0/build/cmake)
add_subdirectory(${PROJECT_ROOT}/externals/glm)

add_executable(app.out main.cpp)

target_include_directories(app.out
    PRIVATE
    ${PROJECT_ROOT}/externals/glfw-3.3.6/include
    ${PROJECT_ROOT}/externals/glew-2.1.0/include
    ${PROJECT_ROOT}/externals/glm/glm
)

include_directories(${PROJECT_ROOT}/includes)
add_subdirectory(${PROJECT_ROOT}/src)

target_link_libraries(app.out
    Modules
    glfw
    glew
    glm
)



