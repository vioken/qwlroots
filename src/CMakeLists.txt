set(TARGET qwlroots)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Qt${QT_VERSION_MAJOR}
    COMPONENTS
    Core
    Gui
    REQUIRED
)

set(WLROOTS_MINIMUM_REQUIRED 0.16.0)
find_package(PkgConfig)
pkg_search_module(WLROOTS REQUIRED IMPORTED_TARGET wlroots>=${WLROOTS_MINIMUM_REQUIRED})
pkg_search_module(WAYLAND_SERVER REQUIRED IMPORTED_TARGET wayland-server)
pkg_search_module(PIXMAN REQUIRED IMPORTED_TARGET pixman-1)
pkg_get_variable(WLR_HAVE_VULKAN wlroots have_vulkan_renderer)

setup_package_version_variables(WLROOTS)
add_definitions(-DWLR_VERSION_STRING="${WLROOTS_VERSION}"
                -DWLR_VERSION_MAJOR=${WLROOTS_VERSION_MAJOR}
                -DWLR_VERSION_MINOR=${WLROOTS_VERSION_MINOR}
                -DWLR_VERSION_PATCH=${WLROOTS_VERSION_PATCH})
message("wlroots version=${WLROOTS_VERSION}, major=${WLROOTS_VERSION_MAJOR}, minor=${WLROOTS_VERSION_MINOR}, patch=${WLROOTS_VERSION_PATCH}")

add_definitions(-DWLR_USE_UNSTABLE -DQW_LIBRARY)

include(${PROJECT_SOURCE_DIR}/cmake/WaylandScannerHelpers.cmake)
ws_generate(server stable/xdg-shell/xdg-shell.xml xdg-shell-protocol)

set(SOURCES
    qwglobal.cpp
    qwbackend.cpp
    qwdisplay.cpp
    render/qwrenderer.cpp
    render/qwtexture.cpp
    render/qwallocator.cpp
    types/qwbuffer.cpp
    types/qwcompositor.cpp
    types/qwsubcompositor.cpp
    types/qwdatadevice.cpp
    types/qwoutputlayout.cpp
    types/qwscene.cpp
    types/qwseat.cpp
    types/qwxdgshell.cpp
    types/qwcursor.cpp
    types/qwxcursormanager.cpp
    types/qwinputdevice.cpp
    types/qwkeyboard.cpp
    types/qwoutput.cpp
    util/qwsignalconnector.cpp
    interfaces/qwbackendinterface.cpp
    interfaces/qwoutputinterface.cpp
)

set(HEADERS
    qwglobal.h
    qwbackend.h
    qwdisplay.h
    render/qwrenderer.h
    render/qwtexture.h
    render/qwallocator.h
    types/qwbuffer.h
    types/qwcompositor.h
    types/qwsubcompositor.h
    types/qwdatadevice.h
    types/qwoutputlayout.h
    types/qwscene.h
    types/qwseat.h
    types/qwxdgshell.h
    types/qwcursor.h
    types/qwxcursormanager.h
    types/qwinputdevice.h
    types/qwkeyboard.h
    types/qwoutput.h
    util/qwsignalconnector.h
    interfaces/qwinterface.h
    interfaces/qwbackendinterface.h
    interfaces/qwoutputinterface.h
)

set(PRIVATE_HEADERS
    types/qwinputdevice_p.h
)

add_library(${TARGET}
    SHARED
    ${SOURCES}
    ${HEADERS}
    ${PRIVATE_HEADERS}
)

set_target_properties(${TARGET}
    PROPERTIES
        VERSION ${CMAKE_PROJECT_VERSION}
        SOVERSION ${PROJECT_VERSION_MAJOR}
        PUBLIC_HEADER "${HEADERS}"
)

set(QT_LIBRAIES
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Gui)

target_link_libraries(${TARGET}
    PRIVATE
    ${QT_LIBRAIES}
    PkgConfig::WLROOTS
    PkgConfig::WAYLAND_SERVER
    PkgConfig::PIXMAN
)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

target_include_directories(${TARGET}
    PRIVATE
    ${Qt${QT_VERSION_MAJOR}Gui_PRIVATE_INCLUDE_DIRS}
)

target_include_directories(${TARGET}
    PUBLIC
)

install(TARGETS ${TARGET}
    LIBRARY
        DESTINATION ${CMAKE_INSTALL_LIBDIR}
    PUBLIC_HEADER
        COMPONENT Development
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${TARGET}
)

include(${PROJECT_SOURCE_DIR}/cmake/Helpers.cmake)
add_cmake_module(QWlroots ${TARGET} ${CMAKE_INSTALL_FULL_INCLUDEDIR}/${TARGET})
add_pkgconfig_module(${TARGET} ${TARGET} ${CMAKE_INSTALL_FULL_INCLUDEDIR}/${TARGET}
    "wlroots >= ${WLROOTS_MINIMUM_REQUIRED}, Qt${QT_VERSION_MAJOR}Gui"
)

install(FILES
    cmake/QWlrootsConfig.cmake
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/QWlroots
)
