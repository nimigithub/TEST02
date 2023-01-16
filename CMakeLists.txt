cmake_minimum_required(VERSION 3.22.0)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(TEST02 VERSION 0.1.0)
set(CMAKE_BUILD_TYPE Debug)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/build/bin)

set(SOURCES main.cpp)

add_executable(TEST02 ${SOURCES})

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE) 
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

set(GLAD_DIR external/glad)
set(GLFW_DIR external/glfw)
set(OPENCV_DIR external/opencv/build_debug)

set(CMAKE_PREFIX_PATH ${OPENCV_DIR})
find_package(OpenCV REQUIRED)

include_directories(${OpenCV_INCLUDE_DIRS})

add_subdirectory(${GLFW_DIR})
add_subdirectory(${GLAD_DIR})

target_include_directories(${PROJECT_NAME}  
    PUBLIC ${GLFW_DIR}/include
    PRIVATE ${GLAD_DIR}/include
    PRIVATE ${OPENCV_DIR}/include
)

### LINK ####

#set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -mwindows")

target_link_directories(${PROJECT_NAME} 
    PUBLIC ${OPENCV_DIR}/lib
)

if (WIN32)
target_link_libraries(${PROJECT_NAME}
    glfw
    glad
    libopencv_core470.dll.a
    libopencv_highgui470.dll.a
    libopencv_imgcodecs470.dll.a
)
else()
target_link_libraries(${PROJECT_NAME}
    glfw
    glad
    libopencv_core.so.407
    libopencv_highgui.so.407
    libopencv_imgcodecs.so.407
)
endif()

