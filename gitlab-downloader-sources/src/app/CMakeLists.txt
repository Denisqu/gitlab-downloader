cmake_minimum_required(VERSION 3.5)
set(CMAKE_EXPORT_COMPILE_COMMANDS O)
project(gitlab-downloader VERSION 0.1 LANGUAGES CXX)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# add_subdirectory(libs/ml-lib)

add_compile_definitions(_ALLOW_ITERATOR_DEBUG_LEVEL_MISMATCH)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(LIBRARY_OUTPUT_PATH "${CMAKE_BINARY_DIR}")
set(EXECUTABLE_OUTPUT_PATH "${CMAKE_BINARY_DIR}")

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Network WebSockets Quick)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Network WebSockets Quick)

set(CMAKE_CXX_FLAGS_COVERAGE "${CMAKE_CXX_FLAGS_RELEASE} -fprofile-arcs -ftest-coverage")

# set up a mapping so that the Release configuration for the Qt imported target is
# used in the COVERAGE CMake configuration.
set_target_properties(Qt5::Core PROPERTIES MAP_IMPORTED_CONFIG_COVERAGE "DEBUG")

add_definitions("-DQPM_INIT\\(E\\)=E.addImportPath\\(QStringLiteral\\(\\\"qrc:/\\\"\\)\\)\\;")

#include(external/material/vendor.cmake)
set(FLUENTUI_DIRECTORY ${CMAKE_CURRENT_LIST_DIR})
option(FLUENTUI_BUILD_EXAMPLES "Build FluentUI demo applications." OFF)
option(FLUENTUI_BUILD_STATIC_LIB "Build static library." OFF)

add_subdirectory(external/fluent)

set(PROJECT_SOURCES
        ${VENDOR_SOURCES}
        main.cpp
        qml/qml.qrc
)

message(root_proj_dir = "${ROOT_PROJECT_DIR}")

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(gitlab-downloader
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET doodle-game APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(gitlab-downloader SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(gitlab-downloader
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(gitlab-downloader PRIVATE
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Network
    Qt${QT_VERSION_MAJOR}::WebSockets
    Qt${QT_VERSION_MAJOR}::Quick
   # db-lib
    fluentuiplugin
)

set_target_properties(gitlab-downloader PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_compile_definitions(gitlab-downloader
  PRIVATE $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(gitlab-downloader)
endif()

