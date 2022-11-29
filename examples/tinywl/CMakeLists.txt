set(TARGET tinywl)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Gui REQUIRED)

set(SOURCES main.cpp)

find_package(PkgConfig)
pkg_search_module(WAYLAND_SERVER REQUIRED wayland-server)
pkg_search_module(WLROOTS REQUIRED wlroots)
pkg_search_module(PIXMAN REQUIRED pixman-1)
pkg_search_module(XKBCOMMON REQUIRED xkbcommon)

include(${PROJECT_SOURCE_DIR}/cmake/WaylandScannerHelpers.cmake)
ws_generate(server stable/xdg-shell/xdg-shell.xml xdg-shell-protocol)

add_executable(${TARGET}
    ${SOURCES}
)

target_link_libraries(${TARGET}
    PRIVATE
    Qt${QT_VERSION_MAJOR}::Gui
    qwlroots
    ${WAYLAND_SERVER_LIBRARIES}
    ${WLROOTS_LIBRARIES}
    ${XKBCOMMON_LIBRARIES}
)

target_include_directories(${TARGET}
    PRIVATE
    ${Qt${QT_VERSION_MAJOR}Gui_PRIVATE_INCLUDE_DIRS}
    ${PIXMAN_INCLUDE_DIRS}
)
