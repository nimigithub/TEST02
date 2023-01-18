cmake_minimum_required(VERSION 3.10.2)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

message(STATUS "Project Source Directory: ${CMAKE_SOURCE_DIR}")
set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/build/bin)
message(STATUS "CMAKE_SOURCE_DIR: ${CMAKE_SOURCE_DIR}")
message(STATUS "CMAKE_BINARY_DIR: ${CMAKE_BINARY_DIR}")


if(EXISTS "${CMAKE_SOURCE_DIR}/Res")
    file(COPY ${CMAKE_SOURCE_DIR}/Res DESTINATION ${CMAKE_BINARY_DIR}/)
else()
    message(FATAL_ERROR "The directory Res does not exist.")
endif()


project(TEST02 VERSION 0.1.0)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/build/bin)

set(SOURCES main.cpp)

add_executable(TEST02 ${SOURCES})

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE) 
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

set(GLAD_DIR external/glad)
set(GLFW_DIR external/glfw)

set(OPENCV_DIR ${PROJECT_SOURCE_DIR}/external/opencv/build_debug)
message(STATUS "OPENCV_DIR: ${OPENCV_DIR}")
set(CMAKE_PREFIX_PATH ${OPENCV_DIR})
set(OpenCV_INCLUDE_DIRS "${OPENCV_DIR}/install/include")
set(OpenCV_LIBS "${OPENCV_DIR}/install/lib")
find_package(OpenCV 4.7 REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

add_subdirectory(${GLFW_DIR})
add_subdirectory(${GLAD_DIR})

target_include_directories(${PROJECT_NAME}  
    PUBLIC ${GLFW_DIR}/include
    PRIVATE ${GLAD_DIR}/include
    PRIVATE ${OpenCV_INCLUDE_DIRS}
)

### LINK ####

if (WIN32)
    target_link_libraries(${PROJECT_NAME}
        glfw
        glad
        opencv_core
        opencv_highgui
        opencv_imgcodecs
    )
else()
    target_link_libraries(${PROJECT_NAME}
        glfw
        glad
        opencv_core
        opencv_highgui
        opencv_imgcodecs
    )
endif()
