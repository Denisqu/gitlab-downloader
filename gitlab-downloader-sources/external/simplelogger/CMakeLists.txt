cmake_minimum_required(VERSION 3.5)

project(simple-logger VERSION 0.1 LANGUAGES CXX)

set(LIBRARY_OUTPUT_PATH "${CMAKE_BINARY_DIR}")
set(EXECUTABLE_OUTPUT_PATH "${CMAKE_BINARY_DIR}")
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)

set(PROJECT_SOURCES
        simplelogger-export.h
        simpleQtLogger.cpp simpleQtLogger.h
        logger.h
)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core Widgets Network Concurrent)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Widgets Network Concurrent)

add_library(simple-logger SHARED
    simplelogger-export.h
    simpleQtLogger.cpp simpleQtLogger.h
    logger.h logger.cpp
)

target_link_libraries(simple-logger PRIVATE
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Network
    Qt${QT_VERSION_MAJOR}::Concurrent
)

target_compile_definitions(simple-logger PRIVATE SIMPLOG_LIBRARY)