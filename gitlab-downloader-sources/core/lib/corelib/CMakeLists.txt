cmake_minimum_required(VERSION 3.14)

project(core-lib LANGUAGES CXX)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(LIBRARY_OUTPUT_PATH "${CMAKE_BINARY_DIR}")
set(EXECUTABLE_OUTPUT_PATH "${CMAKE_BINARY_DIR}")
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core Widgets Network Concurrent Sql)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Widgets Network Concurrent Sql) 
find_package(QCoro6 REQUIRED COMPONENTS Core Network Coro)

add_library(core-lib SHARED
  corelib-export.hpp
  gitlab_handler.cpp gitlab_handler.hpp
  request_formatter.cpp request_formatter.hpp 
  main_table_model.hpp main_table_model.cpp
  reply_handler_interface.hpp
  reply_handler.hpp reply_handler.cpp
)

qcoro_enable_coroutines()

target_link_libraries(core-lib PRIVATE
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Network
    Qt${QT_VERSION_MAJOR}::Concurrent
    Qt${QT_VERSION_MAJOR}::Sql
    QCoro::Core
    QCoro::Network
    QCoro::Coro
    db-lib
)

target_compile_definitions(core-lib PRIVATE CORELIB_LIBRARY)
