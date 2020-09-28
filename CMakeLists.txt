cmake_minimum_required(VERSION 3.10)
project(libalgo VERSION 1.0.0 LANGUAGES CXX)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON) 

list(APPEND SRCS src/IntervalTree.cpp)
# list(APPEND SRCS src/SetOnSplayTree.cpp)
# list(APPEND SRCS src/UkkonensAlgorithm.cpp)

add_library(libalgo ${SRCS})
add_library(malpunek::libalgo ALIAS libalgo)


target_include_directories(libalgo
    PUBLIC 
        $<INSTALL_INTERFACE:include>    
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_compile_options(libalgo PRIVATE -Werror -Wall -Wpedantic $<$<CONFIG:DEBUG>:-O0>)
target_compile_features(libalgo PRIVATE cxx_std_17)

# TODO
# add_subdirectory(tests)
# add_subdirectory(examples)