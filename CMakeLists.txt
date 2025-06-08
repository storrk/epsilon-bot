cmake_minimum_required(VERSION 3.18 FATAL_ERROR)
project(epsilon-bot LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(
    SOURCES
    src/main.cpp
)

include_directories(${CMAKE_SOURCE_DIR}/include)

add_executable(${PROJECT_NAME} ${SOURCES})

# I have done irreversible damage to my machine by installing libtorch in 4 different locations

set(CMAKE_PREFIX_PATH "/home/lich/Repos/libtorch")
set(Caffe2_DIR "${CMAKE_PREFIX_PATH}/share/cmake/Caffe2")
set(Torch_DIR  "${CMAKE_PREFIX_PATH}/share/cmake/Torch")
find_package(Caffe2 REQUIRED CONFIG
             PATHS "${Caffe2_DIR}"
             NO_DEFAULT_PATH)
find_package(Torch REQUIRED CONFIG
             PATHS "${Torch_DIR}"
             NO_DEFAULT_PATH)
message(STATUS "Using Caffe2 from: ${Caffe2_DIR}")
message(STATUS "Using Torch  from: ${Torch_DIR}")

target_include_directories(${PROJECT_NAME}
  PRIVATE
    ${TORCH_INCLUDE_DIRS}
    ${CAFFE2_INCLUDE_DIRS}
)

target_link_libraries(${PROJECT_NAME}
  PRIVATE
    ${TORCH_LIBRARIES}
)

set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 17)
