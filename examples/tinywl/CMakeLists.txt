set(TARGET tinywl)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
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
    qwlroots
    PkgConfig::WAYLAND_SERVER
    PkgConfig::WLROOTS
    PkgConfig::PIXMAN
    PkgConfig::XKBCOMMON
)
