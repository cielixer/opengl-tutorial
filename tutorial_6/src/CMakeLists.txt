project(CXX_PROJECT_TEMPLATE VERSION 1.0)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../includes)

file(GLOB header_dir
	${PROJECT_ROOT}/includes
)

file(GLOB_RECURSE headers
	${header_dir}/*.h
	${header_dir}/*/*.h
	${header_dir}/*.hpp
	${header_dir}/*/*.hpp
)

file(GLOB_RECURSE sources
	${CMAKE_CURRENT_SOURCE_DIR}/*.c
	${CMAKE_CURRENT_SOURCE_DIR}/*/*.c
	${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/*/*.cpp
)

add_library(Modules STATIC ${headers} ${sources})

target_include_directories(Modules
    PRIVATE
    ${PROJECT_ROOT}/externals/glfw-3.3.6/include
    ${PROJECT_ROOT}/externals/glew-2.1.0/include
    ${PROJECT_ROOT}/externals/glm/glm
)

target_link_libraries(Modules
    glfw
    glew
    glm
)