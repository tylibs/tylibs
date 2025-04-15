cmake_minimum_required(VERSION 3.20)

add_library(tyapp INTERFACE)
target_include_directories(tyapp INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
target_sources(tyapp INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/app.cpp)
