cmake_minimum_required(VERSION 3.5)
set(CMAKE_EXPORT_COMPILE_COMMANDS O)
#
project(gitlab-downloader VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(ROOT_PROJECT_DIR ${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(core)
add_subdirectory(widgets-impl)
add_subdirectory(qml-impl)
#add_subdirectory(test)