cmake_minimum_required(VERSION 3.10)
project(libalgo VERSION 1.0.0 LANGUAGES CXX)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON) 

add_library(libalgo INTERFACE)
add_library(malpunek::libalgo ALIAS libalgo)

target_include_directories(libalgo
    INTERFACE 
        $<INSTALL_INTERFACE:include>    
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

# TODO
# add_subdirectory(tests)
add_subdirectory(examples)