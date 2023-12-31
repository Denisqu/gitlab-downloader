cmake_minimum_required(VERSION 3.14)

project(db-lib LANGUAGES CXX)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(LIBRARY_OUTPUT_PATH "${CMAKE_BINARY_DIR}")
set(EXECUTABLE_OUTPUT_PATH "${CMAKE_BINARY_DIR}")
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core Widgets Sql)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Widgets Sql)
find_package(QCoro6 REQUIRED COMPONENTS Core Network Coro)

add_library(db-lib SHARED
  db_manager.cpp db_manager.hpp
  dblib-export.hpp
)

target_link_libraries(db-lib PRIVATE
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Sql
    
)

target_link_libraries(db-lib PUBLIC
  QCoro::Coro
  QCoro::Core
)

target_compile_definitions(db-lib PRIVATE DBLIB_LIBRARY)
