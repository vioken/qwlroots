cmake_minimum_required(VERSION 3.1.0)

project(QWlroots
    VERSION 0.0.1
    LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

option(PREFER_QT_5 "Prefer to use Qt 5 even if we have Qt 6" OFF)

if(PREFER_QT_5)
    find_package(QT NAMES Qt5 REQUIRED COMPONENTS Core)
else()
    find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core)
endif()

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/")
include(PackageVersionHelper)
include(WaylandScannerHelpers)

# For Unix/Linux
include(GNUInstallDirs)

add_subdirectory(src)
add_subdirectory(tests)
add_subdirectory(examples)
