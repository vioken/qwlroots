cmake_minimum_required(VERSION 3.1.0)

project(QWlroots
    VERSION 0.0.1
    LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

option(PREFER_QT_5 "Prefer to use Qt 5 even if we have Qt 6" OFF)

if(PREFER_QT_5)
    find_package(QT NAMES Qt5 REQUIRED COMPONENTS Core)
else()
    find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core)
endif()

# For Unix/Linux
include(GNUInstallDirs)

macro (setup_package_version_variables _packageName)
    if (DEFINED ${_packageName}_VERSION)
        string (REGEX MATCHALL "[0-9]+" _versionComponents "${${_packageName}_VERSION}")
        list (LENGTH _versionComponents _len)
        if (${_len} GREATER 0)
            list(GET _versionComponents 0 ${_packageName}_VERSION_MAJOR)
        endif()
        if (${_len} GREATER 1)
            list(GET _versionComponents 1 ${_packageName}_VERSION_MINOR)
        endif()
        if (${_len} GREATER 2)
            list(GET _versionComponents 2 ${_packageName}_VERSION_PATCH)
        endif()
        if (${_len} GREATER 3)
            list(GET _versionComponents 3 ${_packageName}_VERSION_TWEAK)
        endif()
        set (${_packageName}_VERSION_COUNT ${_len})
    else()
        set (${_packageName}_VERSION_COUNT 0)
        set (${_packageName}_VERSION "")
    endif()
endmacro()

add_subdirectory(src)
add_subdirectory(examples)
