cmake_minimum_required(VERSION 3.14)

project(core-tests LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(LIBRARY_OUTPUT_PATH "${CMAKE_BINARY_DIR}")
set(EXECUTABLE_OUTPUT_PATH "${CMAKE_BINARY_DIR}")

add_executable(core-tests
    test_db_manager.cpp
    test_reply_handler.cpp   
)

target_link_libraries(core-tests PUBLIC
    gtest_main
    db-lib
    core-lib
)

find_package(QT NAMES Qt6 Qt5 REQUIRED
 COMPONENTS Core Widgets Sql TEST Network WebSockets Concurrent)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED
 COMPONENTS Core Widgets Sql TEST Network WebSockets Concurrent)

target_link_libraries(core-tests
    PRIVATE Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Network
    Qt${QT_VERSION_MAJOR}::WebSockets
    Qt${QT_VERSION_MAJOR}::Test
    Qt${QT_VERSION_MAJOR}::Concurrent
)

add_test(
    NAME core-tests
    COMMAND core-tests
)