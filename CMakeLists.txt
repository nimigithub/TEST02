cmake_minimum_required(VERSION 3.0.0)
project(TEST02 VERSION 0.1.0)
set(CMAKE_CXX_STANDARD 20)

set(EXT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/external)
set(SOURCES
    main.cpp
)

add_executable(TEST02 ${SOURCES})

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

set(GLAD_DIR external/glad)
set(GLFW_DIR external/glfw)

add_subdirectory(${GLFW_DIR})
add_subdirectory(${GLAD_DIR})

#add_library(glad ${GLAD_DIR}/src/glad.c)

target_include_directories(${PROJECT_NAME} 
    PUBLIC ${GLFW_DIR}/include
    PRIVATE ${GLAD_DIR}/include
)

# target_link_directories(${PROJECT_NAME} 
#     PUBLIC ${GLFW_DIR}/src
# )

target_link_libraries(${PROJECT_NAME}
    glfw
    glad
)
