cmake_minimum_required(VERSION 3.21..3.27)
project(tinywl)

set(TARGET tinywl)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

if(PROJECT_IS_TOP_LEVEL) # if build as a separated project
    set(CMAKE_CXX_STANDARD 20)
    set(CMAKE_AUTOMOC ON)
    set(CMAKE_AUTORCC ON)

    find_package(QWlroots REQUIRED)

    if(PREFER_QT_5)
        find_package(QT NAMES Qt5 REQUIRED COMPONENTS Core)
    else()
        find_package(QT NAMES Qt6 REQUIRED COMPONENTS Core)
    endif()
else()
    ws_generate(server wayland-protocols stable/xdg-shell/xdg-shell.xml xdg-shell-protocol)
endif()

find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Gui REQUIRED)

set(SOURCES main.cpp)

add_executable(${TARGET}
    ${SOURCES}
)

target_link_libraries(${TARGET}
    PRIVATE
    Qt${QT_VERSION_MAJOR}::Gui
    QWlroots::QWlroots
)
