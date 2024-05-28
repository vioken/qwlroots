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

    function(ws_generate type protocols input_file output_name)
        find_package(PkgConfig)
        pkg_get_variable(WAYLAND_PROTOCOLS ${protocols} pkgdatadir)
        pkg_get_variable(WAYLAND_SCANNER wayland-scanner wayland_scanner)

        if(NOT EXISTS ${input_file})
            set(input_file ${WAYLAND_PROTOCOLS}/${input_file})
        endif()

        execute_process(COMMAND ${WAYLAND_SCANNER}
            ${type}-header
            ${input_file}
            ${CMAKE_CURRENT_BINARY_DIR}/${output_name}.h
        )

        execute_process(COMMAND ${WAYLAND_SCANNER}
            public-code
            ${input_file}
            ${CMAKE_CURRENT_BINARY_DIR}/${output_name}.c
        )
    endfunction()
endif()

find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Gui REQUIRED)

set(SOURCES main.cpp)

find_package(PkgConfig)
pkg_search_module(WAYLAND_SERVER REQUIRED IMPORTED_TARGET wayland-server)
pkg_search_module(WLROOTS REQUIRED IMPORTED_TARGET wlroots)
pkg_search_module(PIXMAN REQUIRED IMPORTED_TARGET pixman-1)
pkg_search_module(XKBCOMMON REQUIRED IMPORTED_TARGET xkbcommon)
pkg_search_module(WAYLAND_PROTOCOLS REQUIRED wayland-protocols)

ws_generate(server wayland-protocols stable/xdg-shell/xdg-shell.xml xdg-shell-protocol)

add_executable(${TARGET}
    ${SOURCES}
)

target_link_libraries(${TARGET}
    PRIVATE
    Qt${QT_VERSION_MAJOR}::Gui
    QWlroots::QWlroots
    PkgConfig::WAYLAND_SERVER
    PkgConfig::WLROOTS
    PkgConfig::PIXMAN
    PkgConfig::XKBCOMMON
)
