cmake_minimum_required(VERSION 3.1)

project(tinyfsm)

add_library(${PROJECT_NAME} INTERFACE)
target_include_directories(${PROJECT_NAME} INTERFACE include)

option(TINYFSM_BUILD_SAMPLES "Build tinyfsm examples" OFF)
if(TINIYFSM_BUILD_SAMPLES)
    add_subdirectory(examples)
endif()
