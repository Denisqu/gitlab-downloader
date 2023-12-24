cmake_minimum_required(VERSION 3.14)

project(core-tests LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(LIBRARY_OUTPUT_PATH "${CMAKE_BINARY_DIR}")
set(EXECUTABLE_OUTPUT_PATH "${CMAKE_BINARY_DIR}")

add_executable(core-tests
    db_manager_tests.cpp
)

target_link_libraries(core-tests PUBLIC
    gtest_main
    db-lib
)

find_package(QT NAMES Qt6 Qt5 REQUIRED
 COMPONENTS Core Widgets Sql TEST Network WebSockets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED
 COMPONENTS Core Widgets Sql TEST Network WebSockets)

target_link_libraries(core-tests
    PRIVATE Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Network
    Qt${QT_VERSION_MAJOR}::WebSockets
    Qt${QT_VERSION_MAJOR}::Test
)

add_test(
    NAME core-tests
    COMMAND core-tests
)